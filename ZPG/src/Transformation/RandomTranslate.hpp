#pragma once

#include "Translate.hpp"
#include <random>
/**
 * @brief Random Translate Transformation
 */
class RandomTranslate : public Translate {
private:
  std::mt19937 rng;

  std::uniform_real_distribution<float> x;
  std::uniform_real_distribution<float> y;
  std::uniform_real_distribution<float> z;

public:
  /**
   * @brief Create Transformation matrix for Tramsalate
   * @param translate Translate by vector
   */
  RandomTranslate()
      : Translate(glm::vec3{1.0}), rng(std::random_device()()), x(0.01, 0.02),
        y(0.01, 0.02), z(0.01, 0.02) {};

  glm::mat4 getMatrix(); ///< Return finalMatrix
};
