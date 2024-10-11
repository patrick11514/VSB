#include "Object.hpp"

Object::Object(ObjectData data, const ShaderProgram &shaderProgram, Transformations transformations, std::function<void()> renderFunction)
    : data(data), shaderProgram(shaderProgram), transformations(transformations), renderFunction(renderFunction)
{
}

void Object::draw() const
{
    this->data.setArray();
    this->shaderProgram.setProgram();

    this->renderFunction();
}
