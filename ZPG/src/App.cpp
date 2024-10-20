#include "App.hpp"
#include "Controller.hpp"
#include "Scenes/Forest.hpp"
#include "Scenes/Objects.hpp"
#include "Shader/Shader.hpp"
#include "Shader/ShaderProgram.hpp"

#include <GLFW/glfw3.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>
#include <optional>
#include <stdexcept>

App::App() : camera(Camera()) { this->controller = new Controller(this); }

App::~App() { delete this->controller; }

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
  glfwSetWindowUserPointer(window, this->controller);
  glfwSetKeyCallback(this->window, [](GLFWwindow *window, int key, int scancode,
                                      int action, int mods) {
    static_cast<Controller *>(glfwGetWindowUserPointer(window))
        ->onKeyPress(window, key, scancode, action, mods);
  });
  glfwSetCursorPosCallback(
      this->window, [](GLFWwindow *window, double x, double y) {
        static_cast<Controller *>(glfwGetWindowUserPointer(window))
            ->onMouse(window, x, y);
      });

  glfwSetWindowSizeCallback(
      this->window, [](GLFWwindow *window, int width, int height) {
        static_cast<Controller *>(glfwGetWindowUserPointer(window))
            ->onResize(window, width, height);
      });
}

void App::createShaders() {
  try {
    this->shaders.addShaderProgram(
        "ColorByCoords",
        new ShaderProgram("../shaders/vertex/ColorByCoords.vert",
                          "../shaders/fragment/ColorByCoords.frag",
                          this->controller));

    this->shaders.addShaderProgram(
        "ColorPurple",
        new ShaderProgram("../shaders/vertex/Purple.vert",
                          "../shaders/fragment/Purple.frag", this->controller));

    this->shaders.addShaderProgram(
        "MatShader", new ShaderProgram("../shaders/vertex/Rotation.vert",
                                       "../shaders/fragment/ColorByCoords.frag",
                                       this->controller));

    this->shaders.addShaderProgram(
        "MatShaderStatic",
        new ShaderProgram("../shaders/vertex/RotationStaticColor.vert",
                          "../shaders/fragment/Purple.frag", this->controller));

    this->shaders.addShaderProgram(
        "blue",
        new ShaderProgram("../shaders/vertex/RotationStaticColor.vert",
                          "../shaders/fragment/Blue.frag", this->controller));
    this->shaders.addShaderProgram(
        "red",
        new ShaderProgram("../shaders/vertex/RotationStaticColor.vert",
                          "../shaders/fragment/Red.frag", this->controller));

    this->shaders.addShaderProgram(
        "green",
        new ShaderProgram("../shaders/vertex/RotationStaticColor.vert",
                          "../shaders/fragment/Green.frag", this->controller));
    this->shaders.addShaderProgram(
        "darkGreen",
        new ShaderProgram("../shaders/vertex/RotationStaticColor.vert",
                          "../shaders/fragment/GreenDark.frag",
                          this->controller));

    this->shaders.addShaderProgram(
        "greenByCoord",
        new ShaderProgram("../shaders/vertex/GreenByCoords.vert",
                          "../shaders/fragment/GreenByCoords.frag",

                          this->controller));

    this->shaders.addShaderProgram(
        "idk", new ShaderProgram("../shaders/vertex/IdkBarvy.vert",
                                 "../shaders/fragment/ColorByCoords.frag",
                                 this->controller));
  } catch (const std::runtime_error &) {
    this->destroy(EXIT_FAILURE);
  }
}

void App::createModels() {
  this->addScene("obj", Objects(this->shaders));
  this->currentScene = "obj";

  this->addScene("forest", Forest(this->shaders));
}

void App::run() {
  glEnable(GL_DEPTH_TEST);

  double prevTime;
  int frames;

  while (!glfwWindowShouldClose(this->window)) {
    // clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (this->currentScene.has_value()) {
      this->getScene(this->currentScene.value()).render();
    }

    ShaderProgram::resetProgram();

    glfwPollEvents();

    this->controller->onFrame();
    // put the stuff we’ve been drawing onto the display
    glfwSwapBuffers(this->window);

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
  glfwDestroyWindow(this->window);
  glfwTerminate();

  exit(status);
}

void App::addScene(const std::string &name, Scene scene) {
  this->scenes.emplace(name, scene);
}

const Scene &App::getScene(const std::string &name) const {
  auto it = this->scenes.find(name);

  if (it == this->scenes.end()) {
    throw std::runtime_error("Cannot find scene");
  }

  return it->second;
}

void App::createWindow() {
  this->window = glfwCreateWindow(800, 600, "ZPG", NULL, NULL);
  if (!this->window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwMakeContextCurrent(this->window);
  glfwSwapInterval(1);

  // start GLEW extension handler
  glewExperimental = GL_TRUE;
  glewInit();

  this->printVersionInfo();

  int width, height;
  glfwGetFramebufferSize(this->window, &width, &height);

  this->calculateProjectionMatrix(width, height);
}

void App::calculateProjectionMatrix(int width, int height, float fov,
                                    float zNear, float zFar) {
  float ratio = width / (float)height;
  this->projectionMatrix =
      glm::perspective(glm::radians(fov), ratio, zNear, zFar);
  glViewport(0, 0, width, height);
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
