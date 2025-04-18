#pragma once

/*
 * File: Translate.hpp
 * Author: Patrik Mintěl (MIN0150)
 * Description: File contains Translate Class
 */

#include "Transformation.hpp"
/**
 * @brief Basic Translate Transformation
 */
class Translate : public Transformation {
protected:
  glm::mat4 finalMatrix;

public:
  /**
   * @brief Create Transformation matrix for Tramsalate
   * @param translate Translate by vector
   */
  Translate(const glm::vec3 &translate);

  glm::mat4 getMatrix(); ///< Return finalMatrix
};
