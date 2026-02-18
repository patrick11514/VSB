#include "Window.hpp"
#include "Camera.hpp"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/trigonometric.hpp>

Window::Window() {
  this->window = glfwCreateWindow(800, 600, "ZPG", NULL, NULL);
  if (!this->window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwMakeContextCurrent(this->window);
  glfwSwapInterval(1);

  // start GLEW extension handler
  glewExperimental = GL_TRUE;
  glewInit();

  glfwGetFramebufferSize(this->window, &this->width, &this->height);
}

void Window::destroy() { glfwDestroyWindow(this->window); }

void Window::updateResolution(int width, int height, const Camera *camera) {
  this->width = width;
  this->height = height;
  this->calculateProjectionMatrix(camera->fov, camera->zNear, camera->zFar);
}

void Window::calculateProjectionMatrix(float fov, float zNear, float zFar) {
  float ratio = width / (float)height;
  this->projectionMatrix =
      glm::perspective(glm::radians(fov), ratio, zNear, zFar);
  glViewport(0, 0, width, height);
}

GLFWwindow *Window::getWindow() const { return this->window; }

glm::mat4 Window::getProjectionMatrix() const { return this->projectionMatrix; }

glm::vec2 Window::getResolution() const {
  return glm::vec2{this->width, this->height};
}
