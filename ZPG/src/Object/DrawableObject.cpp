#include "DrawableObject.hpp"
#include "../Scenes/Scene.hpp"

#include <glm/ext/matrix_float4x4.hpp>

void DrawableObject::assignScene(Scene *scene) {
  BaseObject::assignScene(scene);
  scene->registerProgram(this->shaderProgram);
  this->shaderProgram->registerToCamera(scene);
  this->shaderProgram->registerToLight(scene);
  this->update();
}

void DrawableObject::draw() {
  Drawable::draw();
  this->data->bindArray();

  this->shaderProgram->putModelMatrix(this->getTransformations());

  this->shaderProgram->setProgram();
  this->renderFunction();
  ShaderProgram::resetProgram();
}
