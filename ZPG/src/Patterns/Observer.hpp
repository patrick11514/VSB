#pragma once

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/matrix.hpp>

/**
 * @Brief "Interface" for the observer, which will get viewMatrix from Camera
 * using update function
 */
class Observer {
public:
  virtual ~Observer() = default; ///< Default virtual destructor

  /**
   * @brief This function will get called, when Camera's viewMatrix updates
   * @param matrix new ViewMatrix from camera
   */
  virtual void update(glm::mat4 &matrix) = 0;
};
