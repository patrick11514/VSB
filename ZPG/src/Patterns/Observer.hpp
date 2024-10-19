#pragma once

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/matrix.hpp>

class Observer {
public:
  virtual ~Observer() = default;

  virtual void update(glm::mat4 &matrix) = 0;
  virtual void call() const = 0;
};
