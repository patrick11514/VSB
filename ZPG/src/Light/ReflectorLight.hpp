#pragma once

#include "Light.hpp"
#include <glm/ext/vector_float3.hpp>

class ReflectorLight : public Light {
protected:
  glm::vec3 direction; // TODO: direction should include transformations, so we
                       // point to WEST and rotate by 90deg, so we need to
                       // change direction :)
  float angle;

public:
  ReflectorLight(glm::vec3 color, glm::vec3 direction, float angle,
                 std::shared_ptr<Transformation> transformation)
      : Light(color, transformation, LightType::REFLECTOR),
        direction(direction), angle(angle) {};

  glm::vec3 getDirection() const;
  float getAngle() const;
};
