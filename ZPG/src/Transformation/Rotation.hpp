#pragma once

#include "Transformation.hpp"

class Rotation : public Transformation {
public:
    Rotation(float angle, glm::vec3& axis);
};

