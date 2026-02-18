#pragma once

#include "Material.hpp"
#include <glm/ext/vector_float3.hpp>

class DarkTreeMaterial : public Material {
public:
  DarkTreeMaterial()
      : Material(glm::vec3{0.0, 0.0, 0.0}, glm::vec3{0.0, 1.0, 0.0},
                 glm::vec3{1, 1, 1}, 32.0) {};
};
