#include "ShaderProgram.hpp"

#include <stdio.h>
#include <GLFW/glfw3.h>
#include <stdexcept>

ShaderProgram::ShaderProgram(Shader &vertexShader, Shader &fragmentShader)
{
    this->programId = glCreateProgram();
    glAttachShader(this->programId, fragmentShader.shaderId);
    glAttachShader(this->programId, vertexShader.shaderId);
    glLinkProgram(this->programId);

    GLint status2;
    glGetProgramiv(this->programId, GL_LINK_STATUS, &status2);
    if (status2 == GL_FALSE)
    {
        GLint infoLogLength;
        glGetProgramiv(this->programId, GL_INFO_LOG_LENGTH, &infoLogLength);
        GLchar *strInfoLog = new GLchar[infoLogLength + 1];
        glGetProgramInfoLog(this->programId, infoLogLength, NULL, strInfoLog);
        fprintf(stderr, "Linker failure: %s\n", strInfoLog);
        delete[] strInfoLog;

        throw std::runtime_error("Linker failure");
    }
}

void ShaderProgram::setProgram() const
{
    glUseProgram(this->programId);
}

void ShaderProgram::resetProgram()
{
    glUseProgram(0);
}

bool ShaderProgram::operator==(const ShaderProgram &other) const
{
    return this->programId == other.programId;
}
