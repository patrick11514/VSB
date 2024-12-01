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
  this->shaderProgram->putModelMatrix(this->getTransformations());
  Drawable::update();

  this->data->bindArray();
  this->shaderProgram->setProgram();

  if (this->renderFunction != nullptr) {
    this->renderFunction();
  } else {
    Drawable::draw();
  }

  ShaderProgram::resetProgram();
}
