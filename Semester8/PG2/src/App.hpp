#pragma once

/*
 * File: App.hpp
 * Author: Patrik Mintěl (MIN0150)
 * Description: File contains App class
 */

// Include GLEW
#include <GL/glew.h>
// Include GLFW
#include <GLFW/glfw3.h>

#include <optional>
#include <stdlib.h>
#include <string>
#include <unordered_map>

#include "Object/Texture/TextureStorage.hpp"
#include "Scenes/Scene.hpp"
#include "Shader/ShaderStorage.hpp"
#include "Window.hpp"

// forward declaration
class Controller;

/**
 * @brief Main APP
 */
class App {
private:
  Controller *controller; ///< Controller, which controlls keys/mouse...
  Window *window;         ///< Window object, that manages window

  ShaderStorage shaders;   ///< Storage of all shaders
  TextureStorage textures; ///< Storage of all textures
  std::unordered_map<std::string, Scene *>
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
                Scene *scene); ///< Internally add new Scene called name
  /**
   * @brief Get scene by name
   * @param name Name of scene
   * @return Scene
   * @throws std::runtime_error when scene was not found
   */
  Scene *getScene(const std::string &name);

public:
  App();  ///< Constructor
  ~App(); ///< Destructor

  void initialize();    ///< Initialize window and GLFW
  void prepareScenes(); ///< Create scenes, but not fill them with objects yet
  void createShaders(); ///< Create shaders and store them in ShaderStorage
  void createModels();  ///< Fill predefined scenes with Objects
  void run();           ///< Main render loop
  void
  destroy(int status =
              EXIT_SUCCESS); ///< Destroy full Application and exit with status

  void switchScene(const std::string &name); //< Switch scene

  Scene *getCurrentScene(); ///< Get current scene
  // Controller is our friend
  friend class Controller;
};
