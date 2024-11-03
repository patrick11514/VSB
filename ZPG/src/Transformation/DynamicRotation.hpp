#pragma once

#include "DynamicTransformation.hpp"
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>

/**
 * @brief Dynamic Rotation Transformation
 */
class DynamicRotation : public DynamicTransformation {
private:
  float angle;
  glm::vec3 axes;
  float timeFactor = 1.0f;

public:
  /**
   * @brief Create Transformation matrix for Dynamic Rotation
   * @param angle Angle of rotation
   * @param axes vector which defines axis around which will be rotation
   * performed
   */
  DynamicRotation(float angle, glm::vec3 axes) : angle(angle), axes(axes) {};

  /**
   * @brief Create Transformation matrix for Dynamic Rotation
   * @param angle Angle of rotation
   * @param axes vector which defines axis around which will be rotation
   * @param timeFactor, how much should time modify angle
   * performed
   */
  DynamicRotation(float angle, glm::vec3 axes, float timeFactor)
      : angle(angle), axes(axes), timeFactor(timeFactor) {};

  glm::mat4 getMatrix(float elapsedTime)
      const override; ///< Return finalMatrix multiplied by elapsed time
};
