#include "Transformation.hpp"

Transformation::Transformation(const glm::mat4x4 transformationMatrix)
    : transformtationMatrix(transformationMatrix) {}

glm::mat4x4 Transformation::getMatrix() const {
  return this->transformtationMatrix;
}
