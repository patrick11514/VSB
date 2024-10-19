#include "Camera.hpp"
#include <cstdio>
#include <glm/ext/matrix_transform.hpp>

Camera::Camera() { this->recalculateTarget(); }

void Camera::addObserver(Observer *observer) {
  observer->call();
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
  printf("broadcast start\n");
  printf("list: %zu\n", this->observers.size());
  for (Observer *observer : this->observers) {
    observer->update(newMatrix);
  }
  printf("broadcast end\n");
}

void Camera::recalculateTarget() {
  this->target.x = std::sin(this->pitch) * std::cos(this->yaw);
  this->target.z = std::sin(this->pitch) * std::sin(this->yaw);
  this->target.y = std::cos(this->pitch);

  this->broadcast();
}

void Camera::toLeft() {
  this->eye += glm::normalize(glm::cross(this->target, this->up));
  this->broadcast();
}

void Camera::toRight() {
  this->eye -= glm::normalize(glm::cross(this->target, this->up));

  this->broadcast();
}

void Camera::forward() {
  this->eye += glm::normalize(this->target);

  this->broadcast();
}

void Camera::backward() {
  this->eye -= glm::normalize(this->target);

  this->broadcast();
}

void Camera::changeYaw(float deg) {
  this->yaw += deg;

  printf("Rotating by %f\n", deg);

  if (this->yaw >= 360) {
    this->yaw -= 360;
  }

  this->recalculateTarget();
}
void Camera::changePitch(float deg) {
  this->pitch += deg;

  printf("Rotating by %f\n", deg);

  if (this->pitch > 90.f) {
    this->pitch = 90.f;
  }

  if (this->pitch < -90.f) {
    this->pitch = -90.f;
  }

  if (this->pitch >= 360) {
    this->pitch -= 360;
  }
}
glm::mat4 Camera::calculateViewMatrix() const {

  return glm::lookAt(this->eye, this->target, this->up);
}
