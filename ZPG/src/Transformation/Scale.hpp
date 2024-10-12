#pragma once

#include "Transformation.hpp"

class Scale : public Transformation {
public:
    Scale(const glm::vec3& scale);
};

