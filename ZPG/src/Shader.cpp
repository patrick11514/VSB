#include "Shader.hpp"

Shader::Shader(const char *code, ShaderType shader)
{
    this->shaderId = glCreateShader(shader == ShaderType::Vertex ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);
    glShaderSource(this->shaderId, 1, &code, NULL);
    glCompileShader(this->shaderId);
}
