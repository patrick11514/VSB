#pragma once
// Include GLEW
#include <GL/glew.h>
// Include GLFW
#include <GLFW/glfw3.h>

#include <GLFW/glfw3.h>
#include <glm/ext/matrix_float4x4.hpp>

class Camera;

class Window {
private:
  int width;
  int height;

  GLFWwindow *window;

  glm::mat4 projectionMatrix;

public:
  Window();

  void destroy();
  void calculateProjectionMatrix(float fov, float zNear, float zFar);
  void updateResolution(int width, int height, const Camera *camera);

  glm::mat4 getProjectionMatrix() const;
  GLFWwindow *getWindow() const;
};
