#pragma once

/*
 * File: BezierRotation.hpp
 * Author: Patrik Mintěl (MIN0150)
 * Description: File contains BezierRotation class
 */

#include "Transformation.hpp"
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/matrix.hpp>
#include <stdexcept>

template <typename MatrixType> class BezierMovement : public Transformation {
private:
  MatrixType points;
  float moveDuration; ///< How long does one movement (A->B->C->...) should take

public:
  BezierMovement(MatrixType points, float moveDuration)
      : points(points), moveDuration(moveDuration) {};

  glm::mat4 getMatrix() {
    float time = glfwGetTime();
    float frameTime = std::fmod(time, (this->moveDuration * 2));
    float percentage;
    if (frameTime <= moveDuration) {
      percentage = frameTime / this->moveDuration;
    } else {
      percentage = (this->moveDuration * 2 - frameTime) / this->moveDuration;
    }

    glm::vec3 point;
    if constexpr (std::is_same_v<MatrixType, glm::mat4x3>) {
      point = this->getPowerMatrix<glm::vec4>(percentage) *
              this->getBernsteinBasis<glm::mat4>() *
              glm::transpose(this->points);
    } else if constexpr (std::is_same_v<MatrixType, glm::mat3x3>) {
      point = this->getPowerMatrix<glm::vec3>(percentage) *
              this->getBernsteinBasis<glm::mat3>() *
              glm::transpose(this->points);
    } else {
      throw std::runtime_error("Invalid input matrix");
    }

    return glm::translate(glm::mat4{1}, point);
  }

  template <typename BasisMat> BasisMat getBernsteinBasis() {
    if constexpr (std::is_same_v<MatrixType, glm::mat4x3>) {
      return glm::mat4{glm::vec4(-1.0, 3.0, -3.0, 1.0),
                       glm ::vec4(3.0, -6.0, 3.0, 0),
                       glm ::vec4(-3.0, 3.0, 0, 0), glm ::vec4(1, 0, 0, 0)};
    } else if constexpr (std::is_same_v<MatrixType, glm::mat3x3>) {
      return glm::mat3{glm::vec3(1.0, -2.0, 1.0), glm ::vec3(-2.0, 2.0, 0.0),
                       glm ::vec3(1.0, 0.0, 0)};
    } else {
      throw std::runtime_error("Invalid input matrix");
    }
  }

  template <typename PowerMat> PowerMat getPowerMatrix(float perc) {
    if constexpr (std::is_same_v<MatrixType, glm::mat4x3>) {
      return glm::vec4{perc * perc * perc, perc * perc, perc, 1.0f};
    } else if constexpr (std::is_same_v<MatrixType, glm::mat3x3>) {
      return glm::vec3{perc * perc, perc, 1.0f};
    } else {
      throw std::runtime_error("Invalid input matrix");
    }
  }
};
