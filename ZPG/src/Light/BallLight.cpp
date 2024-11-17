#include "BallLight.hpp"

void BallLight::draw() const {
  Drawable::draw();
  this->data->bindArray();
  this->shaderProgram->putModelMatrix(this->getTransformations());

  this->shaderProgram->setProgram();
  this->renderFunction();
  ShaderProgram::resetProgram();
}
