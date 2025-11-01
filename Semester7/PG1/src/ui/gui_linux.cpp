#include "gui_linux.hpp"

#include <cstdio>
#include <stdexcept>
#include <thread>

// OpenGL headers
#include <GL/glew.h>
#include <GL/gl.h>

// ImGui
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl3.h>

static void gl_check(const char *where)
{
    GLenum e = glGetError();
    if (e != GL_NO_ERROR)
    {
        std::fprintf(stderr, "[GL ERROR] 0x%04x at %s\n", unsigned(e), where);
    }
}

SimpleGuiSDL3::SimpleGuiSDL3(int width, int height)
{
    width_ = width;
    height_ = height;
    if (Init() != 0)
        throw std::runtime_error("Init failed");
}

SimpleGuiSDL3::~SimpleGuiSDL3()
{
    Cleanup();
}

unsigned SimpleGuiSDL3::CreateFloatTexture(int w, int h)
{
    GLuint t = 0;
    glGenTextures(1, &t);
    glBindTexture(GL_TEXTURE_2D, t);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, w, h, 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);
    gl_check("CreateFloatTexture");
    return t;
}

void SimpleGuiSDL3::DestroyTexture(unsigned &tex)
{
    if (tex)
    {
        GLuint t = tex;
        glDeleteTextures(1, &t);
        tex = 0;
    }
}

int SimpleGuiSDL3::Init()
{
    if (SDL_Init(SDL_INIT_VIDEO) == false)
    {
        std::fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return -1;
    }

    // Request GL 3.3 core profile
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    window_ = SDL_CreateWindow("ImGui SDL3 + OpenGL",
                               width_, height_, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY);
    if (!window_)
    {
        std::fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
        return -2;
    }

    glctx_ = SDL_GL_CreateContext(window_);
    if (!glctx_)
    {
        std::fprintf(stderr, "SDL_GL_CreateContext failed: %s\n", SDL_GetError());
        return -3;
    }
    SDL_GL_MakeCurrent(window_, glctx_);
    SDL_GL_SetSwapInterval(1);

    // GLEW (must come after context creation)
    glewExperimental = GL_TRUE;
    GLenum glew_err = glewInit();
    // GLEW can set GL error flag spuriously; clear it.
    glGetError();
    if (glew_err != GLEW_OK)
    {
        std::fprintf(stderr, "glewInit failed: %s\n", glewGetErrorString(glew_err));
        return -4;
    }

    // ImGui
    IMGUI_CHECKVERSION();
    imgui_ctx_ = ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplSDL3_InitForOpenGL(window_, glctx_);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Texture + CPU buffer
    tex_id_ = CreateFloatTexture(width_, height_);

    tex_data_.resize(size_t(width_) * size_t(height_) * 4);
    temp_buffer_.resize(size_t(width_) * size_t(height_) * 4);

    return 0;
}

int SimpleGuiSDL3::Cleanup()
{
    if (imgui_ctx_)
    {
        ImGui::SetCurrentContext(imgui_ctx_);
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext(imgui_ctx_);
        imgui_ctx_ = nullptr;
    }

    DestroyTexture(tex_id_);

    if (glctx_)
    {
        SDL_GL_DestroyContext(glctx_);
        glctx_ = nullptr;
    }
    if (window_)
    {
        SDL_DestroyWindow(window_);
        window_ = nullptr;
    }
    SDL_Quit();
    return 0;
}

void SimpleGuiSDL3::UploadIfDirty()
{
    if (!dirty_.exchange(false))
        return;
    std::lock_guard<std::mutex> lk(tex_data_lock_);
    glBindTexture(GL_TEXTURE_2D, tex_id_);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width_, height_, GL_RGBA, GL_FLOAT, tex_data_.data());
    glBindTexture(GL_TEXTURE_2D, 0);
    gl_check("UploadIfDirty");
}

void SimpleGuiSDL3::Producer()
{
    while (!finish_request_)
    {
        {
            std::lock_guard<std::mutex> lk(temp_buffer_lock_);

            const float t = (frame_ % 600) / 600.0f;

#pragma omp parallel for
            for (int y = 0; y < height_; ++y)
            {
                for (int x = 0; x < width_; ++x)
                {
                    const size_t i = (size_t(y) * size_t(width_) + size_t(x)) * 4;
                    Color4f c = get_pixel(x, y, t);
                    temp_buffer_[i + 0] = c.r;
                    temp_buffer_[i + 1] = c.g;
                    temp_buffer_[i + 2] = c.b;
                    temp_buffer_[i + 3] = c.a;
                }
            }
        }

        {
            std::lock_guard<std::mutex> lk(tex_data_lock_);
            std::lock_guard<std::mutex> lk2(temp_buffer_lock_);

            tex_data_.swap(temp_buffer_);
            dirty_ = true;
        }
    }
}

int SimpleGuiSDL3::MainLoop()
{
    bool running = true;

    frame_ = 0;

    auto producer = std::thread(&SimpleGuiSDL3::Producer, this);

    while (running && !finish_request_)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            ImGui_ImplSDL3_ProcessEvent(&e);
            if (e.type == SDL_EVENT_QUIT)
                running = false;
            if (e.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
                running = false;
            if (e.type == SDL_EVENT_WINDOW_RESIZED)
            {

                std::lock_guard<std::mutex> lk(tex_data_lock_);
                std::lock_guard<std::mutex> lk2(temp_buffer_lock_);

                width_ = e.window.data1;
                height_ = e.window.data2;
                DestroyTexture(tex_id_);
                tex_id_ = CreateFloatTexture(width_, height_);
                tex_data_.resize(size_t(width_) * size_t(height_) * 4);
                temp_buffer_.resize(size_t(width_) * size_t(height_) * 4);
            }
        }

        ++frame_;

        UploadIfDirty();

        // ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        Ui();

        ImGui::Begin("Image");
        ImGui::Image((ImTextureID)(intptr_t)tex_id_, ImVec2((float)width_, (float)height_));
        ImGui::End();

        // Render
        ImGui::Render();
        int display_w = 0, display_h = 0;
        SDL_GetWindowSize(window_, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.05f, 0.05f, 0.07f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window_);
    }

    finish_request_ = true;

    producer.join();

    return 0;
}

int SimpleGuiSDL3::Ui()
{
    return 0;
}

Color4f SimpleGuiSDL3::get_pixel(const int x, const int y, const float t)
{
    // Default: simple gradient that moves in blue channel over time
    const float fx = (width_ > 0) ? float(x) / float(width_) : 0.0f;
    const float fy = (height_ > 0) ? float(y) / float(height_) : 0.0f;
    return Color4f{fx, fy, 0.25f + 0.75f * t, 1.0f};
}
