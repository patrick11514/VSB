#pragma once

/*
 * File: Shader.hpp
 * Author: Patrik Mintěl (MIN0150)
 * Description: File contains Shader Class
 */

#include <GL/glew.h>
#include <string>

/*
 * @brief Type of shader
 */
enum ShaderType { Fragment, Vertex };

/**
 * @brief Class which repesents program (vertex/fragment...) in OpenGL
 */
class Shader {
private:
  GLuint shaderId;     ///< Id of program
  std::string program; ///< Code of program

public:
  /**
   * @brief Basic constructor for Shader
   * @param path to program
   * @param shader type of Shader
   */
  Shader(const char *path, ShaderType shader);

  // ShaderProgram is a friend class of Shader
  friend class ShaderProgram;
};
