#include "Controller.hpp"
#include "App.hpp"
#include "Light/Flashlight.hpp"
#include "Modifiers/Drawable.hpp"
#include "Object/BaseObject.hpp"
#include "Object/Material/TreeMaterial.hpp"
#include "Object/Objects.hpp"
#include "Object/Texture/Texture.hpp"
#include "Transformation/Translate.hpp"
#include "Window.hpp"

#include <GLFW/glfw3.h>
#include <cstdio>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_projection.hpp>

Controller *Controller::instance = nullptr;

Controller *Controller::getInstance(App *app) {
  if (Controller::instance == nullptr) {
    Controller::instance = new Controller(app);
  }

  return Controller::instance;
}

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

    if (key == GLFW_MOUSE_BUTTON_1) {
      // get
      GLuint x = this->cursor.x;
      GLuint y = this->app->window->getResolution().y - this->cursor.y;

      if (this->controlMode == ControlMode::Destroy) {
        GLuint id;
        glReadPixels(x, y, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &id);

        auto *scene = this->app->getCurrentScene();

        if (id > 0) {
          Drawable *object = scene->getObject(id);
          if (object != nullptr)
            scene->removeObject(dynamic_cast<BaseObject *>(object));
        }
      } else if (this->controlMode == ControlMode::Place) {
        GLfloat depth;
        glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
        glm::vec3 screenX = glm::vec3(x, y, depth);

        Window *window = this->app->window;
        glm::vec4 viewPort = glm::vec4(0, 0, window->getResolution());
        glm::vec3 pos =
            glm::unProject(screenX, this->getCamera()->getViewMatrix(),
                           window->getProjectionMatrix(), viewPort);

        auto scene = this->app->getCurrentScene();

        auto tree =
            createTree(this->app->shaders.getShaderProgram("blinnphong").get(),
                       std::make_shared<Transformation>()->addTransformation(
                           new Translate(pos)),
                       std::make_shared<TreeMaterial>());
        tree->assignId();
        scene->addObject(tree);

        printf("%f %f %f\n", pos.x, pos.y, pos.z);
      }
    }
    break;
  case GLFW_RELEASE:
    this->pressedMouseButtons[key] = false;
    break;
  }
}

void Controller::onMouse([[maybe_unused]] GLFWwindow *window, double x,
                         double y) {
  double xDiff = x - this->cursor.x;
  this->cursor.x = x;
  double yDiff = this->cursor.y - y;
  this->cursor.y = y;

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
  for (const int &key : this->pressedKeys) {
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
    case GLFW_KEY_H:
      this->staticSkyBox = true;
      break;
    case GLFW_KEY_J:
      this->staticSkyBox = false;
      break;
    case GLFW_KEY_I: {
      // switch flashligh color
      // Tree remove mode = red
      this->controlMode = ControlMode::Destroy;
      auto lights = this->app->getCurrentScene()->getLights();
      for (auto *light : lights) {
        if (auto flashlight = dynamic_cast<Flashlight *>(light)) {
          flashlight->setColor(glm::vec3{1.0, 0.0, 0.0});
        }
      }
      break;
    }
    case GLFW_KEY_O: {
      // switch flashligh color
      // Tree plan mode = green
      this->controlMode = ControlMode::Place;
      auto lights = this->app->getCurrentScene()->getLights();
      for (auto *light : lights) {
        if (auto flashlight = dynamic_cast<Flashlight *>(light)) {
          flashlight->setColor(glm::vec3{0.0, 1.0, 0.0});
        }
      }
      break;
    }

    case GLFW_KEY_P: {
      // switch flashligh color
      // Zombie bezier = blue
      this->controlMode = ControlMode::Bezier;
      auto lights = this->app->getCurrentScene()->getLights();
      for (auto *light : lights) {
        if (auto flashlight = dynamic_cast<Flashlight *>(light)) {
          flashlight->setColor(glm::vec3{0.0, 0.0, 1.0});
        }
      }
      break;
    }
    }
  }
}

glm::mat4 Controller::getProjectionMatrix() const {
  return this->app->window->getProjectionMatrix();
}

Camera *Controller::getCamera() {
  return this->app->getCurrentScene()->getCamera();
}

bool Controller::getSkyBoxStatic() const { return this->staticSkyBox; }
