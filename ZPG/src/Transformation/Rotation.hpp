#pragma once

#include "Transformation.hpp"
#include <glm/ext/matrix_float4x4.hpp>

class Rotation : public Transformation {
public:
    Rotation(float angle, const glm::vec3& axis);
  //  Rotation(const glm::mat4x4&, float angel, const::glm::vec3 &axis);
};

