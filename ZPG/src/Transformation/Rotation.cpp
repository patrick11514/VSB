#include "Rotation.hpp"
#include "Transformation.hpp"
#include <glm/ext/matrix_transform.hpp>

Rotation::Rotation(float angle, glm::vec3 &rotation)
    : Transformation(glm::rotate(glm::mat4x4{1}, angle, rotation)) {}
