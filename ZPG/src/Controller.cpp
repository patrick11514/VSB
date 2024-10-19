#include "Controller.hpp"

#include <cstdio>

Controller::Controller(App *app) : app(app) {}

void Controller::onKeyPress(GLFWwindow *window, int key, int scancode,
                            int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
  printf("key_callback [%d,%d,%d,%d] \n", key, scancode, action, mods);

  switch (action) {

  case GLFW_PRESS:
    switch (key) {
    case GLFW_KEY_V:
      // App::currentScene = "obj";
      this->app->currentScene = "obj";
      break;
    case GLFW_KEY_B:
      // App::currentScene = "forest";
      this->app->currentScene = "forest";
      break;
    case GLFW_KEY_Q:
      this->app->camera.changeYaw(10.f);
      break;
    case GLFW_KEY_E:
      this->app->camera.changeYaw(-10.f);
      break;
    case GLFW_KEY_W:
      this->app->camera.forward();
      break;
    case GLFW_KEY_S:
      this->app->camera.backward();
      break;
    case GLFW_KEY_A:
      this->app->camera.toLeft();
      break;
    case GLFW_KEY_D:
      this->app->camera.toRight();
      break;
    case GLFW_KEY_R:
      this->app->camera.changePitch(10.f);
      break;
    case GLFW_KEY_T:
      this->app->camera.changePitch(-10.f);
      break;
    }
    break;
  }
}

void Controller::onMouse(GLFWwindow *window, double x, double y) {}

glm::mat4 Controller::getProjectionMatrix() const {
  return this->app->projectionMatrix;
}

Camera &Controller::getCamera() { return this->app->camera; }
