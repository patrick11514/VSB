#include "Object.hpp"
#include <glm/ext/matrix_float4x4.hpp>

Object::Object(std::shared_ptr<ObjectData> data,
               const ShaderProgram *shaderProgram,
               std::shared_ptr<Transformation> transformations,
               RenderFunction renderFunction)
    : data(data), shaderProgram(shaderProgram),
      transformations(transformations), renderFunction(renderFunction) {
  this->animationFunction = [](const glm::mat4x4 &tran, float) { return tran; };
}

Object::Object(std::shared_ptr<ObjectData> data,
               const ShaderProgram *shaderProgram,
               std::shared_ptr<Transformation> transformations,
               RenderFunction renderFunction, AnimationFunction function)
    : Object(data, shaderProgram, transformations, renderFunction) {
  this->animationFunction = function;
}

void Object::setAnimationFunction(AnimationFunction function) {
  this->animationFunction = function;
}

void Object::draw(float time) const {
  this->data->setArray();
  this->shaderProgram->setProgram();

  this->renderFunction(
      this->animationFunction(this->transformations->getMatrix(), time),
      this->shaderProgram);
}
