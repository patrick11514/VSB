#pragma once

#include "Transformation.hpp"

class DynamicTransformation : public Transformation {
public:
  virtual ~DynamicTransformation() = default; ///< Virtual destructor

  using Transformation::getMatrix;
  virtual glm::mat4 getMatrix(float elapsedTime) const;
};
