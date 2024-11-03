#include "DynamicRotation.hpp"
#include <glm/ext/matrix_transform.hpp>

glm::mat4 DynamicRotation::getMatrix(float elapsedTime) const {
  return glm::rotate(glm::mat4{1}, this->angle * elapsedTime * this->timeFactor,
                     this->axes);
}
