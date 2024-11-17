#pragma once

#include "SolidMaterial.hpp"
#include <glm/ext/vector_float3.hpp>

class SolidGreenMaterial : public SolidMaterial {
public:
  SolidGreenMaterial() : SolidMaterial(glm::vec3{0.0, 1.0, 0.0}) {};
};
