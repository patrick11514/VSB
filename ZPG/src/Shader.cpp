#include "Shader.hpp"
#include <fstream>
#include <exception>
#include <iostream>
#include <sstream>

Shader::Shader(const char *path, ShaderType shader)
{
    std::ifstream file(path);
    if (!file)
    {
        throw new std::runtime_error("Unable to open shader file");
    }

    std::stringstream data;
    data << file.rdbuf();

    std::cout << "READER: \n"
              << data.str() << std::endl;

    const char *program = data.str().c_str();
    this->shaderId = glCreateShader(shader == ShaderType::Vertex ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);
    glShaderSource(this->shaderId, 1, &program, NULL);
    glCompileShader(this->shaderId);
}
