#include "DrawableObject.hpp"

#include <glm/ext/matrix_float4x4.hpp>

void DrawableObject::assignScene(Scene *scene) {
  BaseObject::assignScene(scene);
  this->shaderProgram->registerToCamera(scene);
  this->shaderProgram->registerToLight(scene);
}

void DrawableObject::draw() const {
  this->data->setArray();
  this->shaderProgram->setProgram();

  this->shaderProgram->putModelMatrix(this->getTransformations());

  this->renderFunction();
}
