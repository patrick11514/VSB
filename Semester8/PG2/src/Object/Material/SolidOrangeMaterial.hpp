#pragma once

#include "SolidMaterial.hpp"
#include <glm/ext/vector_float3.hpp>

class SolidOrangeMaterial : public SolidMaterial {
public:
  SolidOrangeMaterial() : SolidMaterial(glm::vec3{1.0, 0.5, 0.0}) {};
};
