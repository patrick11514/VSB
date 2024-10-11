#pragma once

#include <glm/matrix.hpp>

class Transformation
{
private:
    glm::mat4x4 transformtationMatrix;

public:
    glm::mat4x4 getMatrix() const;
};