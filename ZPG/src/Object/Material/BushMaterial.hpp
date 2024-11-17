#pragma once

#include "Material.hpp"
#include <glm/ext/vector_float3.hpp>

class BushMaterial : public Material {
public:
  BushMaterial()
      : Material(glm::vec3{0.1, 0.1, 0.1}, glm::vec3{0.0, 0.5, 0.0},
                 glm::vec3{1, 1, 1}, 32.0) {};
};
