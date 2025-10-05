#pragma once

#include <glm/glm.hpp>

class Light {
public:
  glm::vec3 position;
  float intensity;
  glm::vec3 color;

  Light() = default;
  Light(const glm::vec3 &pos, const float I, const glm::vec3 &col)
      : position(pos), intensity(I), color(col) {}

  glm::vec3 radiance() const { return color * intensity; }
};