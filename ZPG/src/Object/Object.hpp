#pragma once

#include "ObjectData.hpp"
#include "../Shader/ShaderProgram.hpp"

class Object
{
private:
    ObjectData &data;
    ShaderProgram &shaderProgram;

public:
    Object(ObjectData &data, ShaderProgram &shaderProgram);
};