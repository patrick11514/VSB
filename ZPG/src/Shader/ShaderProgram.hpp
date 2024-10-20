#pragma once

#include "../Patterns/Observer.hpp"
#include "Shader.hpp"

#include <stdexcept>

class Controller;

/**
 * @brief Shader program is class, which consists of Fragment Shader and Vertex
 * Shader
 */
class ShaderProgram : public Observer {
private:
  GLuint programId;       ///< If of program
  glm::mat4 viewMatrix;   ///< View matrix of window
  Controller *controller; ///< Pointer to controller

public:
  /**
   * Create Shader program from vertex and fragment shader
   * @param vertexShader
   * @param fragmentShader
   * @param controller
   */
  ShaderProgram(const Shader &vertexShader, const Shader &fragmentShader,
                Controller *controller);
  /**
   * Create Shader program directly from paths
   * @param vertexShaderPath Path to vertexShader which will be internally
   * created
   * @param fragmentShader Path to fragmentShader which will be internally
   * created
   * @param controller
   */
  ShaderProgram(const char *vertexShaderPath, const char *fragmentShaderPath,
                Controller *controller);

  void setProgram() const; ///< Set program on gpu

  static void resetProgram(); ///< Reset program on gpu

  /**
   * @brief Check if vertex shader contains Parameter name
   * @param name Name of the parameter
   * @return true of vertex shader acccepts the parameter otherwise returns
   * false
   */
  bool checkParameter(const std::string &name) const;

  /**
   * @brief Fill parameter name with data in value
   * @param name Name of parameter to be filled with value
   * @param value Data which will be put into the parameter
   * @throws std::runtime_exception, if vertex shader doesn't accept name
   * parameter
   */
  template <typename T>
  void putParameter(const std::string &name, T &value) const {
    GLint position = glGetUniformLocation(this->programId, name.c_str());
    if (position == -1) {
      throw std::runtime_error("Unable to find modelMatrix position");
    }

    glUniformMatrix4fv(position, 1, GL_FALSE, &value);
  }

  // operators
  bool operator==(const ShaderProgram &other) const; ///< compare operator

  void
  update(glm::mat4 &viewMatrix) override; ///< Update viewMatrix from Camera
};
