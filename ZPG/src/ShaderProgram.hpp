#pragma once

#include "Shader.hpp"

class ShaderProgram
{
private:
    GLuint programId;

public:
    ShaderProgram(Shader &vertexShader, Shader &fragmentShader);

    void setProgram() const;

    static void resetProgram();

    // operators
    bool operator==(const ShaderProgram &other) const;
};