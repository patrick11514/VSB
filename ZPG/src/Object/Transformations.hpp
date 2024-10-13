#pragma once

#include <glm/matrix.hpp>

#include "../Transformation/Transformation.hpp"

class Transformations
{
private:
    glm::mat4x4 finalTransformation{1};

public:
    Transformations* addTransformation(const Transformation& tran);
    glm::mat4x4 getFinalTransfomarmation() const;
};
