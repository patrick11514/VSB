#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <vector>

class Camera;
class Rasterizer;

class Controller {
private:
  Rasterizer *rasterizer;
  Camera *camera;

  bool keys[1024] = {false};
  glm::vec2 lastMousePos{0, 0};
  bool firstMouse = true;

  static Controller *instance;

  Controller(Rasterizer *rasterizer, Camera* camera);

public:
  float cameraSpeed = 0.1f;

  static Controller *getInstance(Rasterizer *rasterizer = nullptr, Camera* camera = nullptr);
  
  ~Controller();

  void onKeyPress(GLFWwindow *window, int key, int scancode, int action, int mods);
  void onMouse(GLFWwindow *window, double xpos, double ypos);
  void onFrame();
};
