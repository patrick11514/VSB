#include "Scale.hpp"
#include "Transformation.hpp"
#include <glm/ext/matrix_transform.hpp>

Scale::Scale(const glm::vec3 &scale)
    : Transformation(glm::scale(glm::mat4x4{1}, scale)) {}
