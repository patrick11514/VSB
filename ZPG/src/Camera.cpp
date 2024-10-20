#include "Camera.hpp"
#include <cstdio>
#include <glm/ext/matrix_transform.hpp>

Camera::Camera() { this->recalculateTarget(); }

void Camera::addObserver(Observer *observer) {
  this->observers.emplace_back(observer);
}

void Camera::removeObserver(Observer *observer) {
  for (auto begin = this->observers.begin(); begin != this->observers.end();
       ++begin) {
    if (*begin == observer) {
      this->observers.erase(begin);
      return;
    }
  }
}

void Camera::broadcast() const {
  glm::mat4 newMatrix = this->calculateViewMatrix();
  for (Observer *observer : this->observers) {
    observer->update(newMatrix);
  }
}

void Camera::recalculateTarget() {
  double radPitch = glm::radians(this->pitch);
  double radYaw = glm::radians(this->yaw);

  this->target.x = std::cos(radPitch) * std::cos(radYaw);
  this->target.z = std::cos(radPitch) * std::sin(radYaw);
  this->target.y = std::sin(radPitch);

  this->broadcast();
}

void Camera::toLeft(float rate) {
  this->eye -= glm::normalize(glm::cross(this->target, this->up)) * rate;
  this->broadcast();
}

void Camera::toRight(float rate) {
  this->eye += glm::normalize(glm::cross(this->target, this->up)) * rate;

  this->broadcast();
}

void Camera::forward(float rate) {
  this->eye += glm::normalize(this->target) * rate;

  this->broadcast();
}

void Camera::backward(float rate) {
  this->eye -= glm::normalize(this->target) * rate;

  this->broadcast();
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
