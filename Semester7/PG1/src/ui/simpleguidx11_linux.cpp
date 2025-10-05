#include "simpleguidx11_linux.hpp"

#include <cstdio>

SimpleGuiLinux::SimpleGuiLinux(const int width, const int height)
    : width_(width), height_(height) {
  Init();
}

SimpleGuiLinux::~SimpleGuiLinux() { Cleanup(); }

int SimpleGuiLinux::Init() {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) !=
      0) {
    printf("Error: %s\n", SDL_GetError());
    return -1;
  }

  // Create window with OpenGL context
  window_ = SDL_CreateWindow("PG1 Ray Tracer", SDL_WINDOWPOS_CENTERED,
                             SDL_WINDOWPOS_CENTERED, width_, height_,
                             SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
  gl_context_ = SDL_GL_CreateContext(window_);
  SDL_GL_MakeCurrent(window_, gl_context_);
  SDL_GL_SetSwapInterval(1); // Enable vsync

  // Initialize OpenGL loader (GLEW)
  if (glewInit() != GLEW_OK) {
    printf("Failed to initialize OpenGL loader!\n");
    return -1;
  }

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui_ImplSDL3_InitForOpenGL(window_, gl_context_);
  ImGui_ImplOpenGL3_Init("#version 130");

  return 0;
}

void SimpleGuiLinux::Cleanup() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL3_Shutdown();
  ImGui::DestroyContext();

  SDL_GL_DeleteContext(gl_context_);
  SDL_DestroyWindow(window_);
  SDL_Quit();
}

int SimpleGuiLinux::MainLoop() {
  bool running = true;
  SDL_Event event;

  while (running) {
    while (SDL_PollEvent(&event)) {
      ImGui_ImplSDL3_ProcessEvent(&event);
      if (event.type == SDL_QUIT) {
        running = false;
      }
    }

    // Start ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    // Render UI
    ImGui::Begin("Hello, world!");
    ImGui::Text("This is a Linux port!");
    ImGui::End();

    // Rendering
    ImGui::Render();
    glViewport(0, 0, width_, height_);
    glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    SDL_GL_SwapWindow(window_);
  }

  return 0;
}

void SimpleGuiLinux::Producer() {
  // Placeholder for Producer logic from SimpleGuiDX11
}

int SimpleGuiLinux::Ui() {
  // Placeholder for Ui logic from SimpleGuiDX11
  return 0;
}

Color4f SimpleGuiLinux::get_pixel(const int x, const int y, const float t) {
  // Placeholder for get_pixel logic from SimpleGuiDX11
  return Color4f{1.0f, 0.0f, 1.0f, 1.0f};
}

int SimpleGuiLinux::width() const { return width_; }

int SimpleGuiLinux::height() const { return height_; }