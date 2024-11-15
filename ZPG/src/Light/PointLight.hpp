#pragma once

#include "Light.hpp"

class PointLight : public Light {
private:
  float kc; // contant attenuation
  float kl; // linear attenuation
  float kq; // quadratic attenuation
public:
  PointLight(glm::vec3 color, std::shared_ptr<Transformation> transformations,
             float kc, float kl, float kq)
      : Light(color, transformations, LightType::POINT), kc(kc), kl(kl),
        kq(kq) {};

  virtual ~PointLight() = default;

  float getKc() const;
  float getKl() const;
  float getKq() const;
};
