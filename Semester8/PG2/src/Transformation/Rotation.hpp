#pragma once

/*
 * File: Rotation.hpp
 * Author: Patrik Mintěl (MIN0150)
 * Description: File contains Rotation class
 */

#include "Transformation.hpp"
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>

/**
 * @brief Basic Rotation Transformation
 */
class Rotation : public Transformation {
private:
  glm::mat4 finalMatrix; ///< Final matrix created by constructor

public:
  /**
   * @brief Create Transformation matrix for Rotation
   * @param angle Angle of rotation
   * @axes axes vector which defines axis around which will be rotation
   * performed
   */
  Rotation(float angle, glm::vec3 axes);

  glm::mat4 getMatrix(); ///< Return finalMatrix
};
