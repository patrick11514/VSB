#pragma once

#include "Light.hpp"
#include <glm/ext/vector_float3.hpp>

class DirectionalLight : public Light {
private:
  glm::vec3 direction;

public:
  DirectionalLight(glm::vec3 color, glm::vec3 direction, float kc, float kl,
                   float kq)
      : Light(color, std::make_shared<Transformation>(), kc, kl, kq,
              LightType::DIRECTIONAL),
        direction(direction) {};

  glm::vec3 getDirection() const;
};
