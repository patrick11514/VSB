#include "App.hpp"
#include "Controller.hpp"
#include "Scenes/DifferentLight.hpp"
#include "Scenes/Forest.hpp"
#include "Scenes/LightBalls.hpp"
#include "Scenes/Objects.hpp"
#include "Shader/Shader.hpp"
#include "Shader/ShaderProgram.hpp"

#include <GLFW/glfw3.h>
#include <format>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>
#include <optional>
#include <stdexcept>

App::App() { this->controller = new Controller(this); }

App::~App() {
  delete this->controller;
  delete this->window;
  for (auto pair : this->scenes) {
    delete pair.second;
  }
}

void App::error_callback([[maybe_unused]] int error, const char *description) {
  fputs(description, stderr);
}

void App::initialize() {
  glfwSetErrorCallback(error_callback);
  if (!glfwInit()) {
    fprintf(stderr, "ERROR: could not start GLFW3\n");
    exit(EXIT_FAILURE);
  }

  this->createWindow();
  glfwSetWindowUserPointer(this->window->getWindow(), this->controller);
  glfwSetKeyCallback(
      this->window->getWindow(),
      [](GLFWwindow *window, int key, int scancode, int action, int mods) {
        static_cast<Controller *>(glfwGetWindowUserPointer(window))
            ->onKeyPress(window, key, scancode, action, mods);
      });
  glfwSetMouseButtonCallback(
      this->window->getWindow(),
      [](GLFWwindow *window, int key, int action, int mod) {
        static_cast<Controller *>(glfwGetWindowUserPointer(window))
            ->onMouseButton(window, key, action, mod);
      });

  glfwSetCursorPosCallback(
      this->window->getWindow(), [](GLFWwindow *window, double x, double y) {
        static_cast<Controller *>(glfwGetWindowUserPointer(window))
            ->onMouse(window, x, y);
      });

  glfwSetWindowSizeCallback(
      this->window->getWindow(), [](GLFWwindow *window, int width, int height) {
        static_cast<Controller *>(glfwGetWindowUserPointer(window))
            ->onResize(window, width, height);
      });
}

void App::prepareScenes() {
  this->currentScene = "obj";

  this->addScene("obj", new Objects(this->shaders));
  this->addScene("forest", new Forest(this->shaders));
  // this->addScene("light", new LightBalls(this->shaders));
  // this->addScene("different_light", new DifferentLight(this->shaders));
}

void App::createShaders() {
  std::unordered_map<std::string, std::pair<std::string, std::string>>
      shadersToLoad{
          {"ColorsByCoord", {"ColorByCoords.vert", "ColorByCoords.frag"}},
          {"ColorPurple", {"Purple.vert", "Purple.frag"}},
          {"MatShader", {"Rotation.vert", "ColorByCoords.frag"}},
          {"MatShaderStatic", {"RotationStaticColor.vert", "Purple.frag"}},
          {"blue", {"RotationStaticColor.vert", "Blue.frag"}},
          {"red", {"RotationStaticColor.vert", "Red.frag"}},
          {"green", {"RotationStaticColor.vert", "Green.frag"}},
          {"darkGreen", {"RotationStaticColor.vert", "GreenDark.frag"}},
          {"greenByCoord", {"GreenByCoords.vert", "GreenByCoords.frag"}},
          {"lambert", {"Base.vert", "BaseLambert.frag"}},
          {"phong", {"Phong.vert", "Phong.frag"}},
          {"blinnphong", {"Base.vert", "BaseBlinn.frag"}},
      };

  try {
    for (auto &pair : shadersToLoad) {
      this->shaders.addShaderProgram(
          pair.first,
          new ShaderProgram(
              std::format("../shaders/vertex/{}", pair.second.first),
              std::format("../shaders/fragment/{}", pair.second.second),
              this->controller));
    }
  } catch (const std::runtime_error &ex) {
    printf("Shader Exception: %s\n", ex.what());
    this->destroy(EXIT_FAILURE);
  }
}

void App::createModels() {
  for (auto &pair : this->scenes) {
    // if scene is StaticScene, run addObjects on them
    if (dynamic_cast<StaticScene *>(pair.second) != nullptr) {
      static_cast<StaticScene *>(pair.second)->addObjects();
    }
  }
}

void App::run() {
  glEnable(GL_DEPTH_TEST);

  double prevTime;
  int frames;

  while (!glfwWindowShouldClose(this->window->getWindow())) {
    // clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (this->currentScene.has_value()) {
      this->getScene(this->currentScene.value())->render();
    }

    ShaderProgram::resetProgram();

    glfwPollEvents();

    this->controller->onFrame();
    // put the stuff we’ve been drawing onto the display
    glfwSwapBuffers(this->window->getWindow());

    double currentTime = glfwGetTime();
    if (currentTime - prevTime >= 1.0) {
      printf("FPS: %d\n", frames);
      frames = 0;
      prevTime = currentTime;
    }
    ++frames;
  }
}

void App::destroy(int status) {
  this->window->destroy();
  glfwTerminate();

  exit(status);
}

void App::addScene(const std::string &name, Scene *scene) {
  this->scenes.emplace(name, scene);
}

Scene *App::getScene(const std::string &name) {
  auto it = this->scenes.find(name);

  if (it == this->scenes.end()) {
    throw std::runtime_error("Cannot find scene");
  }

  return it->second;
}

void App::createWindow() {
  this->window = new Window();

  this->printVersionInfo();
}

Scene *App::getCurrentScene() {
  if (!this->currentScene.has_value()) {
    throw std::runtime_error("No scene was found");
  }
  return this->getScene(this->currentScene.value());
}

void App::switchScene(const std::string &name) {
  this->currentScene = name;
  auto currentScene = this->getCurrentScene();
  currentScene->getCamera()->notifyObservers();
  for (auto *light : currentScene->getLights()) {
    light->notifyObservers();
  }
}

void App::printVersionInfo() {
  printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
  printf("Using GLEW %s\n", glewGetString(GLEW_VERSION));
  printf("Vendor %s\n", glGetString(GL_VENDOR));
  printf("Renderer %s\n", glGetString(GL_RENDERER));
  printf("GLSL %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
  int major, minor, revision;
  glfwGetVersion(&major, &minor, &revision);
  printf("Using GLFW %i.%i.%i\n", major, minor, revision);
}
