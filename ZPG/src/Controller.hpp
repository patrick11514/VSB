#pragma once

#include "App.hpp"

class Controller {
private:
  App *app;
  // Camera& camera;

  double prevX = -1;
  double prevY = -1;

public:
  Controller(App *app);

  void onKeyPress(GLFWwindow *window, int key, int scancode, int action,
                  int mods);
  void onMouse(GLFWwindow *window, double x, double y);

  glm::mat4 getProjectionMatrix() const;
  Camera &getCamera();
};
