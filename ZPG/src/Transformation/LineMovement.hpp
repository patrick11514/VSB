#pragma once

/*
 * File: LineMovement.hpp
 * Author: Patrik Mintěl (MIN0150)
 * Description: File contains LineMovement class
 */

#include "Transformation.hpp"

class LineMovement : public Transformation {
private:
  glm::vec3 A;        ///< Point A of line
  glm::vec3 B;        ///< Point B of line
  float moveDuration; ///< How long does one movement (A->B) should take
public:
  LineMovement(glm::vec3 A, glm::vec3 B, float moveDuration)
      : A(A), B(B), moveDuration(moveDuration) {};

  glm::mat4 getMatrix();
};
