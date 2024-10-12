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

    GLint status;
    glGetProgramiv(this->programId, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
        printf("ERROR LINK\n");
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

GLint ShaderProgram::getMatrixPosition() const {
    GLint position = glGetUniformLocation(this->programId, "modelMatrix");
    if (position == -1) {
        throw std::runtime_error("Unable to find modelMatrix position");
    }

    return position;
}

bool ShaderProgram::operator==(const ShaderProgram &other) const
{
    return this->programId == other.programId;
}
