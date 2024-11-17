#pragma once

#include "Material.hpp"
#include <glm/ext/vector_float3.hpp>

class BlueLambertMaterial : public Material {
public:
  BlueLambertMaterial()
      : Material(glm::vec3{0.1, 0.1, 0.1}, glm::vec3{0.0, 0.0, 1.0},
                 glm::vec3{0.0}, 32.0) {};
};
