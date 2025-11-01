#pragma once

#include <SDL3/SDL.h>
#include <atomic>
#include <mutex>
#include <vector>

#include "../utils/structs.hpp"

// Forward-declare to keep header light
struct ImGuiContext;

/**
 * SimpleGuiSDL3
 * SDL3 + OpenGL 3.3 + GLEW + Dear ImGui
 * - Creates window + GL context
 * - Maintains an RGBA32F texture and a CPU buffer
 * - Shows the texture via ImGui::Image
 */
class SimpleGuiSDL3
{
public:
  SimpleGuiSDL3(int width, int height);
  ~SimpleGuiSDL3();

  int MainLoop();

private:
  int frame_;

  int Init();
  int Cleanup();
  void UploadIfDirty();

  // GL helpers
  unsigned CreateFloatTexture(int w, int h);
  void DestroyTexture(unsigned &tex);

private:
  // SDL / GL
  SDL_Window *window_{nullptr};
  SDL_GLContext glctx_{nullptr};

  // Texture
  unsigned tex_id_{0}; // OpenGL texture name
  int width_{640};
  int height_{480};

  // CPU buffer (RGBA32F)
  std::vector<float> tex_data_;
  std::mutex tex_data_lock_;
  std::atomic<bool> dirty_{true};

  // Producer thread buffer
  std::vector<float> temp_buffer_;

  // ImGui
  ImGuiContext *imgui_ctx_{nullptr};

  // Control
  std::atomic<bool> finish_request_{false};

  void Producer();

protected:
  virtual int Ui();
  virtual Color4f get_pixel(const int x, const int y, const float t = 0.0f);

  // FPS counter
  std::atomic<float> producer_fps_{0.0f};

  std::mutex temp_buffer_lock_;
};