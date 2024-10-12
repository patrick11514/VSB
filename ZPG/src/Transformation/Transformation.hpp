#pragma once

#include <glm/matrix.hpp>

class Transformation
{
private:
    glm::mat4x4 transformtationMatrix;

public:
    Transformation(const glm::mat4x4 transformationMatrix);

    glm::mat4x4 getMatrix() const;
};
