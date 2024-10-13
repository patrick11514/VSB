#include "Translate.hpp"
#include "Transformation.hpp"
#include <glm/ext/matrix_transform.hpp>

Translate::Translate(const glm::vec3 &scale)
    : Transformation(glm::translate(glm::mat4x4{1}, scale)) {}
