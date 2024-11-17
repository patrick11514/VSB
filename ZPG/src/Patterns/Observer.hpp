#pragma once

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/matrix.hpp>

class Observable;

/**
 * @Brief "Interface" for the observer, which will get viewMatrix from Camera
 * using update function
 */
class Observer {
public:
  virtual ~Observer() = default; ///< Default virtual destructor

  virtual void update(Observable *who) = 0; ///< This function will be called,
                                            ///< when Observable changes
};
