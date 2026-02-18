#include "Translate.hpp"
#include <glm/ext/matrix_transform.hpp>

Translate::Translate(const glm::vec3 &translate)
    : finalMatrix(glm::translate(glm::mat4x4{1}, translate)) {}

glm::mat4 Translate::getMatrix() { return this->finalMatrix; }
