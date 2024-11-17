#include "BallLight.hpp"

void BallLight::draw() {
  this->notifyObservers();

  Drawable::draw();
  this->data->bindArray();
  this->shaderProgram->putModelMatrix(this->getTransformations());

  this->shaderProgram->setProgram();
  this->renderFunction();
  ShaderProgram::resetProgram();
}
