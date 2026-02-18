#include "BallLight.hpp"
#include "Light.hpp"

void BallLight::draw() {
  this->notifyObservers();

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
