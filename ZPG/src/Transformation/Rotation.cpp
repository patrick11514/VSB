#include "Rotation.hpp"
#include <glm/ext/matrix_transform.hpp>

Rotation::Rotation(float angle, glm::vec3 axes)
    : finalMatrix(glm::rotate(glm::mat4{1}, angle, axes)) {}

glm::mat4 Rotation::getMatrix() const { return this->finalMatrix; }
