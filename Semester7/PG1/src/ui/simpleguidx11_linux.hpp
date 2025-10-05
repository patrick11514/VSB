#pragma once

#include "../utils/structs.hpp"
#include <GL/glew.h>
#include <SDL3/SDL.h>
#include <atomic>
#include <chrono>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl3.h>
#include <mutex>

class SimpleGuiLinux {
public:
  SimpleGuiLinux(const int width, const int height);
  ~SimpleGuiLinux();

  int MainLoop();

protected:
  int Init();
  void Cleanup();

  int width() const;
  int height() const;

  void Producer();
  virtual int Ui();
  virtual Color4f get_pixel(const int x, const int y, const float t = 0.0f);

  void UpdateTexture();
  void CreateTexture();

  bool vsync_{true};

private:
  SDL_Window *window_ = nullptr;
  SDL_GLContext gl_context_;
  int width_;
  int height_;

  GLuint texture_id_ = 0;     // OpenGL texture ID
  float *tex_data_ = nullptr; // Buffer for pixel data
  std::mutex tex_data_lock_;  // Mutex for synchronizing access to tex_data_

  std::atomic<bool> finish_request_{false};
};