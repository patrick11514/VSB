#include "Camera.hpp"
#include <cstdio>
#include <glm/ext/matrix_transform.hpp>

Camera::Camera() { this->recalculateTarget(); }

void Camera::recalculateTarget() {
  double radPitch = glm::radians(this->pitch);
  double radYaw = glm::radians(this->yaw);

  this->target.x = std::cos(radPitch) * std::cos(radYaw);
  this->target.z = std::cos(radPitch) * std::sin(radYaw);
  this->target.y = std::sin(radPitch);

  this->notifyObservers();
}

void Camera::toLeft(float rate) {
  this->eye -= glm::normalize(glm::cross(this->target, this->up)) * rate;
  this->notifyObservers();
}

void Camera::toRight(float rate) {
  this->eye += glm::normalize(glm::cross(this->target, this->up)) * rate;

  this->notifyObservers();
}

void Camera::forward(float rate) {
  this->eye += glm::normalize(this->target) * rate;

  this->notifyObservers();
}

void Camera::backward(float rate) {
  this->eye -= glm::normalize(this->target) * rate;

  this->notifyObservers();
}

void Camera::changeYaw(float deg) {
  this->yaw += deg;

  if (this->yaw >= 360) {
    this->yaw -= 360;
  }

  this->recalculateTarget();
}
void Camera::changePitch(float deg) {
  this->pitch += deg;

  if (this->pitch > 90.f) {
    this->pitch = 90.f;
  }

  if (this->pitch < -90.f) {
    this->pitch = -90.f;
  }

  this->recalculateTarget();
}
glm::mat4 Camera::calculateViewMatrix() const {

  return glm::lookAt(this->eye, this->eye + this->target, this->up);
}
