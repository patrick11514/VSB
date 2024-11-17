#pragma once

#include "Light.hpp"

class PointLight : public Light {
public:
  PointLight(glm::vec3 color, std::shared_ptr<Transformation> transformations,
             float kc, float kl, float kq)
      : Light(color, transformations, kc, kl, kq, LightType::POINT) {};

  virtual ~PointLight() = default;
};
