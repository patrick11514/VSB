#include "Shader.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

Shader::Shader(const char *path, ShaderType shader) {
  std::ifstream file(path);
  if (!file) {
    throw std::runtime_error("Unable to open shader file");
  }

  std::stringstream data;
  data << file.rdbuf();

  this->program = data.str();

  const char *program = this->program.c_str();
  this->shaderId = glCreateShader(
      shader == ShaderType::Vertex ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);
  glShaderSource(this->shaderId, 1, &program, NULL);
  glCompileShader(this->shaderId);
}
