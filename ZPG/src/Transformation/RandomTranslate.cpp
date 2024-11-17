#include "RandomTranslate.hpp"
#include <glm/ext/matrix_transform.hpp>

glm::mat4 RandomTranslate::getMatrix() {
  return glm::translate(
      glm::mat4{1.0},
      glm::vec3{this->x(this->rng), this->y(this->rng), this->z(this->rng)});
}
