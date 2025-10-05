#pragma once

#include "../utils/structs.hpp"
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl3.h>

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

  bool vsync_{true};

private:
  SDL_Window *window_ = nullptr;
  SDL_GLContext gl_context_;
  int width_;
  int height_;
};