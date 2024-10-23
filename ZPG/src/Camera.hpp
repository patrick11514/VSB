#pragma once

#include <glm/ext/matrix_transform.hpp>

#include "Patterns/Observable.hpp"
#include "Patterns/Observer.hpp"

/**
 * @brief Camera of our scene
 */
class Camera : public Observable {
private:
  glm::vec3 eye{-1, 0, 0}; ///< Eye position, default 0,0,0
  glm::vec3 target;        ///< Target on which we are looking
  glm::vec3 up{0, 1, 0};   ///< Vector defining where is up

  float pitch = 0; ///< Pitch (-60, 90)
  float yaw = 0;   ///< Yaw (0, 360>

  void recalculateTarget(); ///< Function which will recalculate target
                            ///< position, based of pitch and yaw

public:
  Camera(); ///< Default constructor, which will recalculateTarget

  void toLeft(float rate = 0.1f);   ///< Move eye to left with specific rate
  void toRight(float rate = 0.1f);  ///< Move eye to right with specific rate
  void forward(float rate = 0.1f);  ///< Move eye forward with specific rate
  void backward(float rate = 0.1f); ///< Move eye backward with specific rate
  void changeYaw(float deg);        ///< Change Yaw by degs
  void changePitch(float deg);      ///< Change Pitch by degs

  glm::mat4
  calculateViewMatrix() const; ///< calculateViewMatrix from eye, traget and up
};
