#pragma once

/*
 * File: BallLight.hpp
 * Author: Patrik Mintěl (MIN0150)
 * Description: File contains BallLight class which is just PointLight with ball
 * model
 */

#include "../Modifiers/Drawable.hpp"
#include "../Object/Models/sphere.h"
#include "../Object/ObjectData.hpp"
#include "PointLight.hpp"

#include <glm/ext/vector_float3.hpp>

/**
 * @brief Ball Light, which is PointLight with white ball model
 */
class BallLight : public PointLight, public Drawable {
public:
  /**
   * @brief BallLight constructor
   * @param color Color of light
   * @param transformations Transformations of light and model
   * @param kc Constant attenuation coeficient
   * @param kl Linera attenuation coeficient
   * @param kq Quadratic attenuation coeficient
   * @param shaderProgram Shader Program used to draw this light
   * @param material Material for ball
   */
  BallLight(glm::vec3 color,
            const std::shared_ptr<Transformation> &transformations, float kc,
            float kl, float kq, ShaderProgram *shaderProgram,
            std::shared_ptr<Material> material)
      : PointLight(color, transformations, kc, kl, kq),
        Drawable(std::make_shared<ObjectData>(
                     Model{sphere, sizeof(sphere) / sizeof(float)}, 2,
                     []() {
                       glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                                             6 * sizeof(float), (GLvoid *)0);
                       glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                                             6 * sizeof(float),
                                             (GLvoid *)(3 * sizeof(float)));

                       return 6;
                     }),
                 shaderProgram, material) {};

  void draw() override; ///< Method called on draw
};
