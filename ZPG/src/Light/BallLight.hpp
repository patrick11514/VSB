#pragma once

#include "../Modifiers/Drawable.hpp"
#include "../Object/Models/sphere.h"
#include "../Object/ObjectData.hpp"
#include "Light.hpp"

#include <glm/ext/vector_float3.hpp>

class BallLight : public Light, public Drawable {
public:
  BallLight(glm::vec3 color,
            const std::shared_ptr<Transformation> &transformations, float kc,
            float kl, float kq, ShaderProgram *shaderProgram)
      : Light(color, transformations, kc, kl, kq),
        Drawable(std::make_shared<ObjectData>(
                     Model{sphere, sizeof(sphere) / sizeof(float)}, 2,
                     []() {
                       glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                                             6 * sizeof(float), (GLvoid *)0);
                       glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                                             6 * sizeof(float),
                                             (GLvoid *)(3 * sizeof(float)));
                     }),
                 shaderProgram,
                 []() { glDrawArrays(GL_TRIANGLES, 0, 2880); }) {};

  void draw() const override;
};
