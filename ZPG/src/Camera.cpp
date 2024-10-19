#include "Camera.hpp"
#include <cstdio>
#include <glm/ext/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <iostream>

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
  double radMul = std::numbers::pi / 180;

  this->target.x =
      std::cos(this->pitch * radMul) * std::cos(this->yaw * radMul);
  this->target.z =
      std::cos(this->pitch * radMul) * std::sin(this->yaw * radMul);
  this->target.y = std::sin(this->pitch * radMul);

  std::cout << "Pitch: " << this->pitch << " Yaw: " << this->yaw << std::endl;
  std::cout << "Target: " << glm::to_string(this->target)
            << " Eye: " << glm::to_string(this->eye) << std::endl;

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

  this->recalculateTarget();
}
glm::mat4 Camera::calculateViewMatrix() const {

  return glm::lookAt(this->eye, this->eye + this->target, this->up);
}
