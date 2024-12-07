#pragma once

/*
 * File: RandomRotation.hpp
 * Author: Patrik Mintěl (MIN0150)
 * Description: File contains RandomRotation class
 */

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

  float xCurr;
  float yCurr;
  float zCurr;
  float lastUpdate;

public:
  /**
   * @brief Create Transformation matrix for Tramsalate
   * @param translate Translate by vector
   */
  RandomTranslate(glm::vec3 startTranslation)
      : Translate(startTranslation), rng(std::random_device()()),
        x(-0.001, 0.001), y(-0.0005, 0.001), z(-0.001, 0.001) {};

  glm::mat4 getMatrix(); ///< Return finalMatrix
};
