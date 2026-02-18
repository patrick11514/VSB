#pragma once

/*
 * File: Controller.hpp
 * Author: Patrik Mintěl (MIN0150)
 * Description: File contains Controller class
 */

#include "App.hpp"
#include <GLFW/glfw3.h>
#include <vector>

/**
 * @brief Current cursor position structure
 */
struct Cursor {
  double x;
  double y;
};

/**
 * @brief Struct, which defines state of Controlling of scene
 */
enum ControlMode { Destroy, Place, BezierCubic, BezierQuadratic };

/**
 * @brief Main Controller, which handles keyboard, mouse etc...
 */
class Controller {
private:
  App *app; ///< Pointer to main Application

  std::vector<int> pressedKeys; ///< Vector of all currently pressed keys
  std::unordered_map<int, bool> pressedMouseButtons{
      {GLFW_MOUSE_BUTTON_RIGHT, false},
      {GLFW_MOUSE_BUTTON_LEFT, false},
      {GLFW_MOUSE_BUTTON_MIDDLE, false}}; ///< Map of all currently pressed
                                          ///< mouse button keys

  static Controller *instance; ///< Single instance of Controller

  Controller(App *app); ///< Default constructor which get app pointer

  bool staticSkyBox = false; ///< Should skybox move?

  Cursor cursor;                                  ///< Current cursor position
  ControlMode controlMode = ControlMode::Destroy; ///< Control mode of
                                                  // berizer curve progress
  std::vector<glm::vec3> points;                  // bezier curve points
public:
  /**
   * @brief Singleton constructor, which gets existing instance, or create
   * new one, where it uses the param app
   * @param app only used when creating first instance
   * @retuns Existing or new instance of Controller
   */
  static Controller *getInstance(App *app);

  /**
   * @brief On key press
   * @param window GLFWwindow
   * @param key number of key pressed
   * @param scancode ?
   * @param action if key was pressed/relesed
   * @param mods ?
   */
  void onKeyPress(GLFWwindow *window, int key, int scancode, int action,
                  int mods);
  /**
   * @brief On mouse button press
   * @param window GLFWwindow
   * @param key number of key pressed
   * @param action if key was pressed/relesed
   * @param mod ?
   */
  void onMouseButton(GLFWwindow *window, int key, int action, int mod);
  /**
   * @brief On mouse move
   * @param window GLFWwindow
   * @param x X position of mouse
   * @param y Y position of mouse
   */
  void onMouse(GLFWwindow *window, double x, double y);
  /**
   * @brief On window resize
   * @param window GLFWwindow
   * @param width width of window
   * @param height height of window
   */
  void onResize(GLFWwindow *window, int width, int height);
  /**
   * @brief On Frame. This functions run, on every render frame,
   * so we can handle movement etc.. in it
   */
  void onFrame(); ///< This function is called every frame in main App loop
                  ///
  glm::mat4 getProjectionMatrix() const; ///< Get projection matrix from APP
  Camera *getCamera();                   ///< Get camera from Current scene
  bool getSkyBoxStatic() const;
};
