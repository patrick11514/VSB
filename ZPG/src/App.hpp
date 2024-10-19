#pragma once

// Include GLEW
#include <GL/glew.h>
// Include GLFW
#include <GLFW/glfw3.h>

#include <optional>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unordered_map>

#include "Camera.hpp"
#include "Scenes/Scene.hpp"
#include "Shader/ShaderStorage.hpp"

class Controller;

class App {
private:
  GLFWwindow *window;
  Controller *controller;
  Camera camera;

  ShaderStorage shaders;
  std::unordered_map<std::string, Scene> scenes;
  std::optional<std::string> currentScene;

  void createWindow();
  void printVersionInfo();

  // callbacks
  static void error_callback(int error, const char *description);

  void addScene(const std::string &name, Scene scene);
  const Scene &getScene(const std::string &name) const;

  glm::mat4 projectionMatrix;

public:
  App();
  ~App();

  void initialize();
  void createShaders();
  void createModels();
  void run();
  void destroy(int status = EXIT_SUCCESS);

  friend class Controller;
};
