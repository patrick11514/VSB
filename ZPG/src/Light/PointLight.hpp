#pragma once

/*
 * File: PointLight.hpp
 * Author: Patrik Mintěl (MIN0150)
 * Description: File includes Point light class
 */

#include "Light.hpp"

/**
 * @brief Point light
 */
class PointLight : public Light {
public:
  /**
   * @brief PointLight constructor
   * @param color Light color
   * @param transformations Transformations of light (position primarly)
   * @param kc Constant attenuation coeficient
   * @param kl Linera attenuation coeficient
   * @param kq Quadratic attenuation coeficient
   */
  PointLight(glm::vec3 color, std::shared_ptr<Transformation> transformations,
             float kc, float kl, float kq)
      : Light(color, transformations, kc, kl, kq, LightType::POINT) {};

  virtual ~PointLight() = default;
};
