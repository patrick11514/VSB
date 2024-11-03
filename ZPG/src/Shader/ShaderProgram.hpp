#pragma once

#include "../Camera.hpp"
#include "../Patterns/Observer.hpp"
#include "../Transformation/Transformation.hpp"
#include "Shader.hpp"

#include <stdexcept>

class Controller;
class Scene;

/**
 * @brief Shader program is class, which consists of Fragment Shader and Vertex
 * Shader
 */
class ShaderProgram : public Observer {
private:
  GLuint programId;       ///< If of program
  Controller *controller; ///< Pointer to controller
  Camera *camera;         ///< Vector to pointer to camera

  /**
   * @brief Fill parameter name with data in value
   * @param name Name of parameter to be filled with value
   * @param value Data which will be put into the parameter
   * @throws std::runtime_exception, if vertex shader doesn't accept name
   * parameter
   */
  template <typename T>
  void putParameter(const std::string &name, T *value) const {
    GLint position = glGetUniformLocation(this->programId, name.c_str());
    if (position == -1) {
      throw std::runtime_error("Unable to find modelMatrix position");
    }

    glUniformMatrix4fv(position, 1, GL_FALSE, value);
  }

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

  void registerToCamera(Scene *scene);

  // operators
  bool operator==(const ShaderProgram &other) const; ///< compare operator

  void update() override; ///< Update viewMatrix from Camera

  void putModelMatrix(const glm::mat4 &matrix) const;
  void putProjectionMatrix(const glm::mat4 &matrix) const;
  void putViewMatrix(const glm::mat4 &matrix) const;
  void putCameraPosition(const glm::vec3 &vector) const;
};
