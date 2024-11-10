#include "BallLight.hpp"

void BallLight::draw() const {
  printf("Drawing light\n");
  this->data->setArray();
  this->shaderProgram->setProgram();

  this->shaderProgram->putModelMatrix(this->getTransformations());

  this->renderFunction();
}
