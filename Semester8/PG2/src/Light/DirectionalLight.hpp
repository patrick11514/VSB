#pragma once

/*
 * File: DirectionalLight.hpp
 * Author: Patrik Mintěl (MIN0150)
 * Description: File contains DirectionalLight class
 */

#include "Light.hpp"
#include <glm/ext/vector_float3.hpp>

/**
 * @brief Directional Light is special type of light, which lights from one
 * direction and is "infinitely" distanced
 */
class DirectionalLight : public Light {
private:
  glm::vec3 direction; ///< Direction, which light is pointing

public:
  /**
   * @brief Directional Light constructor
   * @param color Color of light
   * @param direction Direction of Directional light
   * @param kc Constant attenuation coeficient
   * @param kl Linera attenuation coeficient
   * @param kq Quadratic attenuation coeficient
   */
  DirectionalLight(glm::vec3 color, glm::vec3 direction, float kc, float kl,
                   float kq)
      : Light(color, std::make_shared<Transformation>(), kc, kl, kq,
              LightType::DIRECTIONAL),
        direction(direction) {};

  glm::vec3 getDirection() const; ///< Direction
};
