#pragma once

#include "SolidMaterial.hpp"
#include <glm/ext/vector_float3.hpp>

class SolidBlueMaterial : public SolidMaterial {
public:
  SolidBlueMaterial() : SolidMaterial(glm::vec3{0.0, 0.0, 1.0}) {};
};
