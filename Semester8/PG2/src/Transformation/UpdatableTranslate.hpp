#pragma once

/*
 * File: UpdatableTranslate.hpp
 * Author: Patrik Mintěl (MIN0150)
 * Description: File contains UpdatableTranslate class
 */

#include "./Translate.hpp"

/**
 * @brief Basic Translate Transformation
 */
class UpdatableTranslate : public Translate {
public:
  /**
   * @brief Create Transformation matrix for Tramsalate
   * @param translate Translate by vector
   */
  UpdatableTranslate(const glm::vec3 &translate) : Translate(translate) {};

  void update(const glm::vec3 &translate);
};
