#pragma once

#include <glm/ext/matrix_transform.hpp>

#include "Object/BaseObject.hpp"
#include "Patterns/Observable.hpp"
#include "Patterns/Observer.hpp"
#include "Window.hpp"

/**
 * @brief Camera of our scene
 */
class Camera : public BaseObject, public Observable {
private:
  glm::vec3 eye{-1, 0, 0}; ///< Eye position, default 0,0,0
  glm::vec3 target;        ///< Target on which we are looking
  glm::vec3 up{0, 1, 0};   ///< Vector defining where is up

  float pitch = 0; ///< Pitch (-60, 90)
  float yaw = 0;   ///< Yaw (0, 360>

  float fov;
  float zNear;
  float zFar;

  glm::mat4 viewMatrix;

  bool enabled = false;

public:
  Camera(float fov = 60.f, float zNear = 0.1f,
         float zFar =
             100.f); ///< Default constructor, which will recalculateTarget

  void recalculateTarget(); ///< Function which will recalculate target
                            ///< position, based of pitch and yaw

  void toLeft(float rate = 0.1f);   ///< Move eye to left with specific rate
  void toRight(float rate = 0.1f);  ///< Move eye to right with specific rate
  void forward(float rate = 0.1f);  ///< Move eye forward with specific rate
  void backward(float rate = 0.1f); ///< Move eye backward with specific rate
  void changeYaw(float deg);        ///< Change Yaw by degs
  void changePitch(float deg);      ///< Change Pitch by degs

  void calculateViewMatrix(); ///< calculateViewMatrix from eye, traget and up
  glm::mat4 getViewMatrix() const;

  void notifyObservers();

  glm::vec3 getPosition() const; // getCurrentPosition of eye
  glm::vec3 getTarget() const;

  void enable();
  void disabled();
  bool getStatus() const;

  friend class Window;
};
