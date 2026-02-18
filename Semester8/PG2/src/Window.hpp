#pragma once

/*
 * File: Window.hpp
 * Author: Patrik Mintěl (MIN0150)
 * Description: File contains Window class
 */

// Include GLEW
#include <GL/glew.h>
// Include GLFW
#include <GLFW/glfw3.h>

#include <GLFW/glfw3.h>
#include <glm/ext/matrix_float4x4.hpp>

// forward declaration
class Camera;

/**
 * @brief Window class
 */
class Window {
private:
  int width;  ///< width of window
  int height; ///< height of window

  GLFWwindow *window; ///< Pointer to current window

  glm::mat4 projectionMatrix;

public:
  Window();

  void destroy(); ///< Destroy GLFWwindow and cleanup
  void calculateProjectionMatrix(float fov, float zNear, float zFar);
  void updateResolution(int width, int height, const Camera *camera);

  glm::mat4 getProjectionMatrix() const;
  GLFWwindow *getWindow() const;
  glm::vec2 getResolution() const;
};
