#include "Camera.hpp"
#include <GLFW/glfw3.h>
#include <cstdio>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

Camera::Camera(float fov, float zNear, float zFar)
    : fov(fov), zNear(zNear), zFar(zFar) {
  this->recalculateTarget();
}

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
}

void Camera::changePitch(float deg) {
  this->pitch += deg;

  if (this->pitch >= 90.f) {
    this->pitch = 89.f;
  }

  if (this->pitch <= -90.f) {
    this->pitch = -89.f;
  }
}

void Camera::calculateViewMatrix() {
  this->viewMatrix = glm::lookAt(this->eye, this->eye + this->target, this->up);
}

glm::mat4 Camera::getViewMatrix() const { return this->viewMatrix; }

void Camera::notifyObservers() {
  this->calculateViewMatrix();
  Observable::notifyObservers();
}

glm::vec3 Camera::getPosition() const { return this->eye; }

void Camera::enable() { this->enabled = true; }

void Camera::disabled() { this->enabled = false; }

bool Camera::getStatus() const { return this->enabled; }
