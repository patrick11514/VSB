#pragma once

#include "../Camera.hpp"
#include "../Light/Light.hpp"
#include "../Patterns/Observer.hpp"
#include "../ShaderLoader/ShaderLoader.h"
#include "../Transformation/Transformation.hpp"
#include "Shader.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <stdexcept>
#include <string>

class Controller;
class Scene;

/**
 * @brief Shader program is class, which consists of Fragment Shader and Vertex
 * Shader
 */
class ShaderProgram : public ShaderLoader, public Observer {
public:                   //@TODO RESET
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
  void putParameter(const std::string &name, const T &value) const {
    GLint position = glGetUniformLocation(this->programId, name.c_str());
    if (position == -1) {
      printf("Unable to find %s position\n", name.c_str());
      return;
    }

    this->setProgram();
    if constexpr (std::is_same<T, glm::mat4>::value) {
      glUniformMatrix4fv(position, 1, GL_FALSE, glm::value_ptr(value));
    } else if constexpr (std::is_same<T, glm::vec3>::value) {
      glUniform3fv(position, 1, glm::value_ptr(value));
    } else if constexpr (std::is_same<T, float>::value) {
      glUniform1f(position, value);
    } else if constexpr (std::is_same<T, int>::value) {
      glUniform1i(position, value);
    } else if constexpr (std::is_same<T, glm::vec4>::value) {
      glUniform4fv(position, 1, glm::value_ptr(value));
    } else {
      throw std::runtime_error("Passed invalid type to " + name);
    }
    this->resetProgram();
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

  ShaderProgram(const std::string &vertexShaderPath,
                const std::string &fragmentShaderPath, Controller *controller)
      : ShaderProgram(vertexShaderPath.c_str(), fragmentShaderPath.c_str(),
                      controller) {};

  void setProgram() const; ///< Set program on gpu

  static void resetProgram(); ///< Reset program on gpu

  /**
   * @brief Check if vertex shader contains Parameter name
   * @param name Name of the parameter
   * @return true of vertex shader acccepts the parameter otherwise returns
   * false
   */
  bool checkParameter(const std::string &name) const;

  void registerToCamera(Scene *scene); /// Change to registerToCameras
  void registerToLight(Scene *scene);  /// Change to registerToLights

  // operators
  bool operator==(const ShaderProgram &other) const; ///< compare operator

  void
  update(const Observable *who) override; ///< Update viewMatrix from Camera

  void putModelMatrix(const Transformation *transformations) const;
  void putProjectionMatrix(const glm::mat4 &matrix) const;
  void putViewMatrix(const glm::mat4 &matrix) const;
  void putCameraPosition(const glm::vec3 &vector) const;
  void putLightPosition(const Light *light) const;
  void putLightProperties(const Light *light) const;
};
