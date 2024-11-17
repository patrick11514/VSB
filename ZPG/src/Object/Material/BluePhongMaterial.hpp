#pragma once

#include "Material.hpp"
#include <glm/ext/vector_float3.hpp>

class BluePhongMaterial : public Material {
public:
  BluePhongMaterial()
      : Material(glm::vec3{0.1, 0.1, 0.1}, glm::vec3{0.0, 0.0, 1.0},
                 glm::vec3{1.0}, 32.0) {};
};
