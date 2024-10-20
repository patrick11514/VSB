#pragma once

#include "Transformation.hpp"
/**
 * @brief Basic Scale Transformation
 */
class Scale : public Transformation {
  glm::mat4 finalMatrix; ///< Final matrix created by constructor

public:
  /**
   * @brief Create Transformation matrix for Scale
   * @param scale Scale in axes
   */
  Scale(const glm::vec3 &scale);

  glm::mat4 getMatrix() const; ///< Return finalMatrix
};
