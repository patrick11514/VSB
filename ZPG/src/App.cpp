#include "App.hpp"
#include "Object/ObjectData.hpp"
#include "Object/Objects.hpp"
#include "Object/Transformations.hpp"
#include "Scenes/Forest.hpp"
#include "Shader/Shader.hpp"
#include "Shader/ShaderProgram.hpp"
#include "Transformation/Rotation.hpp"
#include "Transformation/Scale.hpp"
#include "Transformation/Translate.hpp"

#include <GLFW/glfw3.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>
#include <optional>
#include <stdexcept>

std::optional<std::string> App::currentScene = std::nullopt;

void App::error_callback(int error, const char *description) {
  fputs(description, stderr);
}

void App::initialize() {
  glfwSetErrorCallback(error_callback);
  if (!glfwInit()) {
    fprintf(stderr, "ERROR: could not start GLFW3\n");
    exit(EXIT_FAILURE);
  }

  this->createWindow();
  glfwSetKeyCallback(this->window, key_callback);
}

void App::key_callback(GLFWwindow *window, int key, int scancode, int action,
                       int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
  printf("key_callback [%d,%d,%d,%d] \n", key, scancode, action, mods);

  if (key == GLFW_KEY_A && action == GLFW_PRESS) {
    App::currentScene = "obj";
  } else if (key == GLFW_KEY_B && action == GLFW_PRESS) {
    App::currentScene = "forest";
  }
}

void App::createShaders() {
  try {
    Shader vertexShader("../shaders/vertex/ColorByCoords.vert",
                        ShaderType::Vertex);
    Shader fragmentShader("../shaders/fragment/ColorByCoords.frag",
                          ShaderType::Fragment);

    ShaderProgram shaderProgram(vertexShader, fragmentShader);
    this->shaders.addShaderProgram("ColorByCoords", shaderProgram);

    Shader vertexShader2("../shaders/vertex/Purple.vert", ShaderType::Vertex);

    Shader fragmentShader2("../shaders/fragment/Purple.frag",
                           ShaderType::Fragment);

    ShaderProgram shaderProgram2(vertexShader2, fragmentShader2);
    this->shaders.addShaderProgram("ColorPurple", shaderProgram2);

    Shader vertexMat("../shaders/vertex/Rotation.vert", ShaderType::Vertex);

    ShaderProgram shaderMat(vertexMat, fragmentShader);

    this->shaders.addShaderProgram("MatShader", shaderMat);

    Shader vertexMatStatic("../shaders/vertex/RotationStaticColor.vert",
                           ShaderType::Vertex);
    ShaderProgram shaderMatStatic(vertexMatStatic, fragmentShader2);

    this->shaders.addShaderProgram("MatShaderStatic", shaderMatStatic);

    Shader fragmentBlue("../shaders/fragment/Blue.frag", ShaderType::Fragment);
    Shader fragmentRed("../shaders/fragment/Red.frag", ShaderType::Fragment);

    ShaderProgram blueProgram(vertexMatStatic, fragmentBlue);
    ShaderProgram redProgram(vertexMatStatic, fragmentRed);
    this->shaders.addShaderProgram("blue", blueProgram);
    this->shaders.addShaderProgram("red", redProgram);

    Shader fragmentGreen("../shaders/fragment/Green.frag",
                         ShaderType::Fragment);
    Shader fragmentDarkGreen("../shaders/fragment/GreenDark.frag",
                             ShaderType::Fragment);

    ShaderProgram greenProgram(vertexMatStatic, fragmentGreen);
    ShaderProgram darkGreenProgram(vertexMatStatic, fragmentDarkGreen);

    this->shaders.addShaderProgram("green", greenProgram);
    this->shaders.addShaderProgram("darkGreen", darkGreenProgram);

    Shader fragmentGreenByCoord("../shaders/fragment/GreenByCoords.frag",
                                ShaderType::Fragment);
    Shader vertexGreenByCoord("../shaders/vertex/GreenByCoords.vert",
                              ShaderType::Vertex);
    ShaderProgram greenByCoord(vertexGreenByCoord, fragmentGreenByCoord);

    this->shaders.addShaderProgram("greenByCoord", greenByCoord);

    Shader idkBarvyVertex("../shaders/vertex/IdkBarvy.vert",
                          ShaderType::Vertex);

    ShaderProgram idkBarvy(idkBarvyVertex, fragmentShader);
    this->shaders.addShaderProgram("idk", idkBarvy);
  } catch (const std::runtime_error &) {
    this->destroy(EXIT_FAILURE);
  }
}

void App::createModels() {
  /*Scene scene;*/
  /**/
  /*Model model({0.5f, 0.5f,  0.0f, 0.5f,  -0.5f, 0.0f, -0.5f, -0.5f,*/
  /*             0.0f, -0.5f, 0.5f, 0.0f,  0.75f, 1.0f, 0.0f,  1.0f,*/
  /*             0.5f, 0.0f,  0.5f, 0.5f,  0.0f,  0.0f, 0.5f,  0.0f,*/
  /*             0.5f, -0.5f, 0.0f, -0.5f, -0.5f, 0.0f});*/
  /*ObjectData data(model);*/
  /**/
  /*scene.addObject(Object(data,
   * this->shaders.getShaderProgram("ColorByCoords"),*/
  /*                       Transformations(),*/
  /*                       [](const glm::mat4x4 &, const ShaderProgram &) {*/
  /*                         glDrawArrays(GL_TRIANGLES, 4, 3);*/
  /*                       }));*/
  /**/
  /*scene.addObject(Object(data,
   * this->shaders.getShaderProgram("ColorPurple"),*/
  /*                       Transformations(),*/
  /*                       [](const glm::mat4x4 &, const ShaderProgram &) {*/
  /*                         glDrawArrays(GL_QUADS, 0, 4);*/
  /*                       }));*/
  /**/
  /*this->addScene("objects", scene);*/
  /**/
  Scene objScene;

  Transformations tran;

  /*tran.addTransformation(Rotation(1.f, vec));*/
  tran.addTransformation(Scale(glm::vec3(0.2f)))
      ->addTransformation(Translate(glm::vec3(1.f, 0.f, 0.f)));

  objScene.addObject(
      createBall(this->shaders.getShaderProgram("MatShader"), tran));

  Transformations tran2;
  tran2.addTransformation(Translate(glm::vec3(-0.5f, 0.5f, 0.f)));

  objScene.addObject(createBush(this->shaders.getShaderProgram("blue"), tran2));

  Transformations tran3;
  tran3.addTransformation(Scale(glm::vec3(.1f)));

  objScene.addObject(createTree(this->shaders.getShaderProgram("red"), tran3));

  this->addScene("obj", objScene);

  App::currentScene = "obj";

  this->addScene("forest", Forest(this->shaders));
}

void App::run() {
  glEnable(GL_DEPTH_TEST);
  while (!glfwWindowShouldClose(this->window)) {
    // clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (this->currentScene.has_value()) {
      this->getScene(this->currentScene.value()).render();
    }

    ShaderProgram::resetProgram();

    glfwPollEvents();
    // put the stuff we’ve been drawing onto the display
    glfwSwapBuffers(this->window);
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

  glfwMakeContextCurrent(this->window);
  glfwSwapInterval(1);

  // start GLEW extension handler
  glewExperimental = GL_TRUE;
  glewInit();

  this->printVersionInfo();

  int width, height;
  glfwGetFramebufferSize(this->window, &width, &height);
  float ratio = width / (float)height;
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
