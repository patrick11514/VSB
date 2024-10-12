#pragma once
#include <GL/glew.h>
#include <string>

enum ShaderType
{
    Fragment,
    Vertex
};

class Shader
{
private:
    GLuint shaderId;
    std::string program;

public:
    Shader(const char *path, ShaderType shader);

    // ShaderProgram is a friend class of Shader
    friend class ShaderProgram;
};
