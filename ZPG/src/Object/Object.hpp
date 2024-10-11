#pragma once

#include <functional>

#include "ObjectData.hpp"
#include "Transformations.hpp"
#include "../Shader/ShaderProgram.hpp"

class Object
{
private:
    ObjectData data;
    const ShaderProgram &shaderProgram;
    Transformations transformations;
    std::function<void()> renderFunction;

public:
    Object(ObjectData data, const ShaderProgram &shaderProgram, Transformations transformations, std::function<void()> renderFunction);

    // set shader, set data and apply transformations
    void draw() const;
};