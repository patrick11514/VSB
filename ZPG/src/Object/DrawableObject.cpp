#include "DrawableObject.hpp"

#include <glm/ext/matrix_float4x4.hpp>

void DrawableObject::assignScene(Scene *scene) {
  BaseObject::assignScene(scene);
  this->shaderProgram->registerToCamera(scene);
  this->shaderProgram->registerToLight(scene);
  this->update();
}

void DrawableObject::update() {
  this->shaderProgram->putMaterial(this->material.get());
}

void DrawableObject::draw() const {
  this->data->bindArray();

  this->shaderProgram->putModelMatrix(this->getTransformations());

  this->shaderProgram->setProgram();
  this->renderFunction();
  ShaderProgram::resetProgram();
}
