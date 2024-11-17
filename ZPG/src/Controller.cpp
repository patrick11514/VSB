#include "Controller.hpp"
#include "App.hpp"

#include <GLFW/glfw3.h>
#include <cstdio>
#include <glm/ext/matrix_clip_space.hpp>

Controller::Controller(App *app) : app(app) {}

void Controller::onKeyPress(GLFWwindow *window, int key, int scancode,
                            int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
  printf("key_callback [%d,%d,%d,%d] \n", key, scancode, action, mods);

  switch (action) {
  case GLFW_PRESS:
    this->pressedKeys.emplace_back(key);
    break;
  case GLFW_RELEASE:
    printf("%d RELASE\n", key);
    for (auto begin = this->pressedKeys.begin();
         begin != this->pressedKeys.end(); ++begin) {
      if (*begin == key) {
        this->pressedKeys.erase(begin);
        break;
      }
    }
    break;
  }
}

void Controller::onMouseButton([[maybe_unused]] GLFWwindow *window, int key,
                               int action, int mod) {
  printf("mouse_button_callback [%d,%d,%d] \n", key, action, mod);

  switch (action) {
  case GLFW_PRESS:
    this->pressedMouseButtons[key] = true;
    break;
  case GLFW_RELEASE:
    this->pressedMouseButtons[key] = false;
    break;
  }
}

void Controller::onMouse([[maybe_unused]] GLFWwindow *window, double x,
                         double y) {
  double xDiff = x - this->prevX;
  this->prevX = x;
  double yDiff = this->prevY - y;
  this->prevY = y;

  if (!this->pressedMouseButtons[GLFW_MOUSE_BUTTON_RIGHT])
    return;

  auto camera = this->getCamera();

  camera->changeYaw(xDiff / 4);
  camera->changePitch(yDiff / 4);
  camera->recalculateTarget();
}

void Controller::onResize([[maybe_unused]] GLFWwindow *window, int width,
                          int height) {
  printf("resize: %dx%d\n", width, height);
  this->app->window->updateResolution(width, height, this->getCamera());
  this->getCamera()->notifyObservers();
}

void Controller::onFrame() {
  for (const int key : this->pressedKeys) {
    switch (key) {
    case GLFW_KEY_V:
      // App::currentScene = "obj";
      this->app->switchScene("obj");
      break;
    case GLFW_KEY_B:
      // App::currentScene = "forest";
      this->app->switchScene("forest");
      break;
    case GLFW_KEY_N:
      // App::currentScene = "forest";
      this->app->switchScene("light");
      break;
    case GLFW_KEY_M:
      // App::currentScene = "forest";
      this->app->switchScene("different_light");
      break;
    case GLFW_KEY_X:
      // App::currentScene = "forest";
      this->app->switchScene("dark_forest");
      break;
    case GLFW_KEY_W:
      this->getCamera()->forward();
      break;
    case GLFW_KEY_S:
      this->getCamera()->backward();
      break;
    case GLFW_KEY_A:
      this->getCamera()->toLeft();
      break;
    case GLFW_KEY_D:
      this->getCamera()->toRight();
      break;
    }
  }
}

glm::mat4 Controller::getProjectionMatrix() const {
  return this->app->window->getProjectionMatrix();
}

Camera *Controller::getCamera() {
  return this->app->getCurrentScene()->getCamera();
}
