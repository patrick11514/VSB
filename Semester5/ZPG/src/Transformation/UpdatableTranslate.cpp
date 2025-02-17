#include "UpdatableTranslate.hpp"
#include <glm/ext/matrix_transform.hpp>

void UpdatableTranslate::update(const glm::vec3 &translate) {
  this->finalMatrix = glm::translate(glm::mat4{1.0}, translate);
}
