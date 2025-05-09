#pragma once

#include "Material.hpp"
#include <glm/ext/vector_float3.hpp>

class DarkBushMaterial : public Material {
public:
  DarkBushMaterial()
      : Material(glm::vec3{0.0, 0.0, 0.0}, glm::vec3{0.0, 0.5, 0.0},
                 glm::vec3{1.0}, 32.0) {};
};
