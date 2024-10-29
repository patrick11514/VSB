#pragma once

#include "App.hpp"
#include <GLFW/glfw3.h>

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

  double prevX = 0; ///< previous x position of mouse
  double prevY = 0; ///< previsout y position of mouse
public:
  Controller(App *app); ///< Default constructor which get app pointer

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

  void onFrame(); ///< This function is called every frame in main App loop

  glm::mat4 getProjectionMatrix() const; ///< Get projection matrix from APP
  Camera *getCamera();                   ///< Get camera from Current scene
};
