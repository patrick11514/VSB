#pragma once

#include "Material.hpp"

class SolidMaterial : public Material {
public:
  SolidMaterial(glm::vec3 color)
      : Material(color, glm::vec3{0.0}, glm::vec3{0.0}, 32) {}
};
