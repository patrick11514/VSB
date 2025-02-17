#pragma once

#include "SolidMaterial.hpp"
#include <glm/ext/vector_float3.hpp>

class SolidRedMaterial : public SolidMaterial {
public:
  SolidRedMaterial() : SolidMaterial(glm::vec3{1.0, 0.0, 0.0}) {};
};
