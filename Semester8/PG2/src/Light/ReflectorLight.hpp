#pragma once

/*
 * File: ReflectorLight.hpp
 * Author: Patrik Mintěl (MIN0150)
 * Description: File contains ReflectorLight class
 */

#include "Light.hpp"
#include <glm/ext/vector_float3.hpp>

/**
 * @brief Reflector light
 */
class ReflectorLight : public Light {
protected:
  glm::vec3 direction; ///< Direction of Reflector, which doesn't account
                       ///< transformations (I was lazy :/)
  float angle;         ///< Angle of reflector

public:
  /**
   * @brief ReflectorLight constructor
   * @param color Light color
   * @param direction Direction of light
   * @param angle Angle of light cone
   * @param transformations Transformations of light (position primarly)
   * @param kc Constant attenuation coeficient
   * @param kl Linera attenuation coeficient
   * @param kq Quadratic attenuation coeficient
   */
  ReflectorLight(glm::vec3 color, glm::vec3 direction, float angle,
                 std::shared_ptr<Transformation> transformation, float kc,
                 float kl, float kq)
      : Light(color, transformation, kc, kl, kq, LightType::REFLECTOR),
        direction(direction), angle(angle) {};

  glm::vec3 getDirection() const;
  float getAngle() const;
};
