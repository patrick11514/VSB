#pragma once

#include "Transformation.hpp"
/**
 * @brief Basic Translate Transformation
 */
class Translate : public Transformation {
private:
  glm::mat4 finalMatrix;

public:
  /**
   * @brief Create Transformation matrix for Tramsalate
   * @param translate Translate by vector
   */
  Translate(const glm::vec3 &translate);

  glm::mat4 getMatrix() const; ///< Return finalMatrix
};
