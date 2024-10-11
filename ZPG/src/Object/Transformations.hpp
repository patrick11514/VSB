#pragma once

#include <glm/matrix.hpp>

class Transformations
{
private:
    glm::mat4x4 finalTransformation{1};

public:
    void addTransformation();
};