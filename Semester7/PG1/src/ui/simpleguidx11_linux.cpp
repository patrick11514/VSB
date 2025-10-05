#include "simpleguidx11_linux.hpp"

#include <cstdio>
#include <thread>

SimpleGuiLinux::SimpleGuiLinux(const int width, const int height)
    : width_(width), height_(height) {
  Init();
}

SimpleGuiLinux::~SimpleGuiLinux() {
  Cleanup();
  delete[] tex_data_;
  tex_data_ = nullptr;
}

int SimpleGuiLinux::Init() {
  if (!SDL_InitSubSystem(SDL_INIT_VIDEO)) {
    printf("Error: %s\n", SDL_GetError());
    return -1;
  }

  // Create window with OpenGL context
  window_ = SDL_CreateWindow("PG1 Ray Tracer", width_, height_,
                             SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
  if (!window_) {
    printf("Failed to create SDL window: %s\n", SDL_GetError());
    SDL_Quit();
    return -1;
  }

  gl_context_ = SDL_GL_CreateContext(window_);
  if (!gl_context_) {
    printf("Failed to create OpenGL context: %s\n", SDL_GetError());
    SDL_DestroyWindow(window_);
    SDL_Quit();
    return -1;
  }

  SDL_GL_MakeCurrent(window_, gl_context_);
  SDL_GL_SetSwapInterval(1); // Enable vsync

  // Initialize OpenGL loader (GLEW)
  if (glewInit() != GLEW_OK) {
    printf("Failed to initialize OpenGL loader!\n");
    SDL_GL_DestroyContext(gl_context_);
    SDL_DestroyWindow(window_);
    SDL_Quit();
    return -1;
  }

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  if (!ImGui_ImplSDL3_InitForOpenGL(window_, gl_context_)) {
    printf("Failed to initialize ImGui for SDL and OpenGL!\n");
    ImGui::DestroyContext();
    SDL_GL_DestroyContext(gl_context_);
    SDL_DestroyWindow(window_);
    SDL_Quit();
    return -1;
  }

  if (!ImGui_ImplOpenGL3_Init("#version 130")) {
    printf("Failed to initialize ImGui OpenGL3 backend!\n");
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
    SDL_GL_DestroyContext(gl_context_);
    SDL_DestroyWindow(window_);
    SDL_Quit();
    return -1;
  }

  tex_data_ = new float[width_ * height_ * 4];
  CreateTexture();

  return 0;
}

void SimpleGuiLinux::Cleanup() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL3_Shutdown();
  ImGui::DestroyContext();

  SDL_GL_DestroyContext(gl_context_);
  SDL_DestroyWindow(window_);
  SDL_Quit();
}

int SimpleGuiLinux::MainLoop() {
  std::thread producer_thread(&SimpleGuiLinux::Producer, this);

  bool running = true;
  SDL_Event event;

  while (running) {
    while (SDL_PollEvent(&event)) {
      ImGui_ImplSDL3_ProcessEvent(&event);
      if (event.type == SDL_EVENT_QUIT) {
        running = false;
      }
    }

    // Start ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    // Render UI
    Ui();

    ImGui::Begin("Image", 0, ImGuiWindowFlags_NoResize);
    ImGui::Image(reinterpret_cast<void *>(texture_id_),
                 ImVec2(float(width_), float(height_)));
    ImGui::End();

    UpdateTexture();

    // Rendering
    ImGui::Render();
    glViewport(0, 0, width_, height_);
    glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    SDL_GL_SwapWindow(window_);
  }

  finish_request_.store(true, std::memory_order_release);
  producer_thread.join();

  return 0;
}

void SimpleGuiLinux::Producer() {
  float *local_data = new float[width_ * height_ * 4];

  float t = 0.0f; // time
  auto t0 = std::chrono::high_resolution_clock::now();

  while (!finish_request_.load(std::memory_order_acquire)) {
    auto t1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> dt = t1 - t0;
    t += dt.count();
    t0 = t1;

    // Compute rendering
    for (int y = 0; y < height_; ++y) {
      for (int x = 0; x < width_; ++x) {
        const Color4f pixel = get_pixel(x, y, t);
        const int offset = (y * width_ + x) * 4;

        local_data[offset] = pixel.r;
        local_data[offset + 1] = pixel.g;
        local_data[offset + 2] = pixel.b;
        local_data[offset + 3] = pixel.a;
      }
    }

    // Write rendering results
    {
      std::lock_guard<std::mutex> lock(tex_data_lock_);
      memcpy(tex_data_, local_data, width_ * height_ * 4 * sizeof(float));
    }
  }

  delete[] local_data;
}

int SimpleGuiLinux::Ui() {
  ImGui::Begin("Image", nullptr, ImGuiWindowFlags_NoResize);
  ImGui::Image(reinterpret_cast<void *>(texture_id_),
               ImVec2(static_cast<float>(width_), static_cast<float>(height_)));
  ImGui::End();
  return 0;
}

Color4f SimpleGuiLinux::get_pixel(const int x, const int y, const float t) {
  return Color4f{1.0f, 0.0f, 1.0f, 1.0f};
}

void SimpleGuiLinux::CreateTexture() {
  glGenTextures(1, &texture_id_);
  glBindTexture(GL_TEXTURE_2D, texture_id_);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width_, height_, 0, GL_RGBA,
               GL_FLOAT, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void SimpleGuiLinux::UpdateTexture() {
  std::lock_guard<std::mutex> lock(tex_data_lock_);

  if (SDL_GL_GetCurrentContext() != gl_context_) {
    printf("Error: OpenGL context is not active!\n");
    return;
  }

  glBindTexture(GL_TEXTURE_2D, texture_id_);

  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    printf("Error before glTexSubImage2D: %d\n", error);
    return;
  }

  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width_, height_, GL_RGBA, GL_FLOAT,
                  tex_data_);

  error = glGetError();
  if (error != GL_NO_ERROR) {
    printf("Error after glTexSubImage2D: %d\n", error);
  }
}