#pragma once

#include "SolidMaterial.hpp"
#include <glm/ext/vector_float3.hpp>

class SolidWhiteMaterial : public SolidMaterial {
public:
  SolidWhiteMaterial() : SolidMaterial(glm::vec3{1.0, 1.0, 1.0}) {};
};
