#pragma once

#include <glm/ext/matrix_transform.hpp>

class Camera {
private:
  glm::vec3 eye{0.0f, 0.0f, 3.0f}; ///< Eye position
  glm::vec3 target;                ///< Target on which we are looking
  glm::vec3 up{0, 1, 0};           ///< Vector defining where is up

  float pitch = 0; ///< Pitch (-90, 90)
  float yaw = -90;   ///< Yaw (0, 360)

  float fov;
  float zNear;
  float zFar;

  glm::mat4 viewMatrix;

public:
  Camera(float fov = 60.f, float zNear = 0.1f, float zFar = 100.f);

  void recalculateTarget();

  void toLeft(float rate = 0.5f);
  void toRight(float rate = 0.5f);
  void forward(float rate = 0.5f);
  void backward(float rate = 0.5f);
  void upMovement(float rate = 0.5f);
  void downMovement(float rate = 0.5f);
  void changeYaw(float deg);
  void changePitch(float deg);

  void calculateViewMatrix();
  glm::mat4 getViewMatrix() const;

  glm::vec3 getPosition() const;
  glm::vec3 getTarget() const;

  float getFov() const { return fov; }
  float getZNear() const { return zNear; }
  float getZFar() const { return zFar; }
};
