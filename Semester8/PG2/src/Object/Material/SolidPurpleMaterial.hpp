#pragma once

#include "SolidMaterial.hpp"
#include <glm/ext/vector_float3.hpp>

class SolidPurpleMaterial : public SolidMaterial {
public:
  SolidPurpleMaterial() : SolidMaterial(glm::vec3{1.0, 0.0, 1.0}) {};
};
