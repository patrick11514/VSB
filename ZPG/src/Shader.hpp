#pragma once
#include <GL/glew.h>

enum ShaderType
{
    Fragment,
    Vertex
};

class Shader
{
private:
    GLuint shaderId;

public:
    Shader(const char *code, ShaderType shader);

    // ShaderProgram is a friend class of Shader
    friend class ShaderProgram;
};