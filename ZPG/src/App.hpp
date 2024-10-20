#pragma once

// Include GLEW
#include <GL/glew.h>
// Include GLFW
#include <GLFW/glfw3.h>

#include <optional>
#include <stdlib.h>
#include <string>
#include <unordered_map>

#include "Camera.hpp"
#include "Scenes/Scene.hpp"
#include "Shader/ShaderStorage.hpp"

// forward declaration
class Controller;

/**
 * @brief Main APP
 */
class App {
private:
  GLFWwindow *window;     ///< GLFW window
  Controller *controller; ///< Controller, which controlls keys/mouse...
  Camera camera;          ///< Main Camera

  ShaderStorage shaders; ///< Storage of all shaders
  std::unordered_map<std::string, Scene>
      scenes;                              ///< Key-Value storage of all scenes
  std::optional<std::string> currentScene; ///< Current Scene

  void createWindow();     ///< Function to create default window
  void printVersionInfo(); ///< Funtion to print information about GPU, version
                           ///< of OpenGL....

  static void error_callback(
      int error,
      const char *description); ///< Error callback from GLFW, only callback
                                ///< here, because it doesn't controll anything,
                                ///< so it sayed here

  void addScene(const std::string &name,
                Scene scene); ///< Internally add new Scene called name
  /**
   * @brief Get scene by name
   * @param name Name of scene
   * @return Scene
   * @throws std::runtime_error when scene was not found
   */
  const Scene &getScene(const std::string &name) const;

  glm::mat4 projectionMatrix; ///< Projection matrix of window

public:
  App();  ///< Constructor
  ~App(); ///< Destructor

  void initialize();    ///< Initialize window and GLFW
  void createShaders(); ///< Create shaders and store them in ShaderStorage
  void createModels();  ///< Create Models and scenes
  void run();           ///< Main render loop
  void
  destroy(int status =
              EXIT_SUCCESS); ///< Destroy full Application and exit with status

  // Controller is our friend
  friend class Controller;
};
