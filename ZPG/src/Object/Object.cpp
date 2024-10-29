#include "Object.hpp"
#include "../Scenes/Scene.hpp"
#include <glm/ext/matrix_float4x4.hpp>

Object::Object(std::shared_ptr<ObjectData> data, ShaderProgram *shaderProgram,
               std::shared_ptr<Transformation> transformations,
               RenderFunction renderFunction)
    : data(data), shaderProgram(shaderProgram),
      transformations(transformations), renderFunction(renderFunction) {
  this->animationFunction = [](const glm::mat4x4 &tran, float) { return tran; };
}

Object::Object(std::shared_ptr<ObjectData> data, ShaderProgram *shaderProgram,
               std::shared_ptr<Transformation> transformations,
               RenderFunction renderFunction, AnimationFunction function)
    : Object(data, shaderProgram, transformations, renderFunction) {
  this->animationFunction = function;
}

void Object::setAnimationFunction(AnimationFunction function) {
  this->animationFunction = function;
}

void Object::assignScene(Scene *scene) {
  this->myScene = scene;
  this->shaderProgram->registerToCamera(scene);
}

void Object::draw(float time) const {
  this->data->setArray();
  this->shaderProgram->setProgram();

  this->renderFunction(
      this->animationFunction(this->transformations->getMatrix(), time),
      this->shaderProgram);
}
