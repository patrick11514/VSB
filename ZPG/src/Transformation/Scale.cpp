#include "Scale.hpp"
#include <glm/ext/matrix_transform.hpp>

Scale::Scale(const glm::vec3 &scale)
    : finalMatrix(glm::scale(glm::mat4x4{1}, scale)) {}

glm::mat4 Scale::getMatrix() { return this->finalMatrix; }
