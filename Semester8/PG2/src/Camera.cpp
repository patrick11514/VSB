#include "Camera.hpp"
#include <glm/ext/matrix_clip_space.hpp>

Camera::Camera(float fov, float zNear, float zFar)
    : fov(fov), zNear(zNear), zFar(zFar) {
  this->recalculateTarget();
}

void Camera::recalculateTarget() {
  // Task 1: derive the forward vector from the current yaw and pitch.
  double radPitch = glm::radians(this->pitch);
  double radYaw = glm::radians(this->yaw);

  this->target.x = std::cos(radPitch) * std::cos(radYaw);
  this->target.y = std::sin(radPitch);
  this->target.z = std::cos(radPitch) * std::sin(radYaw);

  this->target = glm::normalize(this->target);
  this->calculateViewMatrix();
}

// Task 1: camera movement helpers used by the keyboard controller.
void Camera::toLeft(float rate) {
  this->eye -= glm::normalize(glm::cross(this->target, this->up)) * rate;
  this->calculateViewMatrix();
}

void Camera::toRight(float rate) {
  this->eye += glm::normalize(glm::cross(this->target, this->up)) * rate;
  this->calculateViewMatrix();
}

void Camera::forward(float rate) {
  glm::vec3 planarForward =
      glm::normalize(glm::vec3(this->target.x, 0.0f, this->target.z));
  this->eye += planarForward * rate;
  this->calculateViewMatrix();
}

void Camera::backward(float rate) {
  glm::vec3 planarForward =
      glm::normalize(glm::vec3(this->target.x, 0.0f, this->target.z));
  this->eye -= planarForward * rate;
  this->calculateViewMatrix();
}

void Camera::upMovement(float rate) {
  this->eye += this->up * rate;
  this->calculateViewMatrix();
}

void Camera::downMovement(float rate) {
  this->eye -= this->up * rate;
  this->calculateViewMatrix();
}

// Task 1: mouse look updates the camera orientation for free movement.
void Camera::changeYaw(float deg) {
  this->yaw += deg;
  this->recalculateTarget();
}

void Camera::changePitch(float deg) {
  this->pitch += deg;

  if (this->pitch > 89.0f) {
    this->pitch = 89.0f;
  }
  if (this->pitch < -89.0f) {
    this->pitch = -89.0f;
  }
  this->recalculateTarget();
}

// Task 1: build the view matrix from the current camera position and target.
void Camera::calculateViewMatrix() {
  glm::vec3 e = this->eye;
  glm::vec3 t = this->eye + this->target;
  glm::vec3 upVec = this->up;

  glm::vec3 ze = glm::normalize(e - t);
  glm::vec3 xe = glm::normalize(glm::cross(upVec, ze));
  glm::vec3 ye = glm::cross(ze, xe);

  this->viewMatrix = glm::mat4(xe.x, ye.x, ze.x, 0.0f, xe.y, ye.y, ze.y, 0.0f,
                               xe.z, ye.z, ze.z, 0.0f, -glm::dot(xe, e),
                               -glm::dot(ye, e), -glm::dot(ze, e), 1.0f);
}

glm::mat4 Camera::getViewMatrix() const { return this->viewMatrix; }

// Task 1: build the projection matrix used for clip-space transforms.
glm::mat4 Camera::getProjectionMatrix(float ratio) const {
  float tanHalfFovy = std::tan(glm::radians(this->fov) / 2.0f);
  glm::mat4 projection(0.0f);
  projection[0][0] = 1.0f / (ratio * tanHalfFovy);
  projection[1][1] = 1.0f / (tanHalfFovy);
  projection[2][2] = (this->zNear + this->zFar) / (this->zNear - this->zFar);
  projection[3][2] =
      (2.0f * this->zNear * this->zFar) / (this->zNear - this->zFar);
  projection[2][3] = -1.0f;
  return projection;
}

glm::vec3 Camera::getPosition() const { return this->eye; }

glm::vec3 Camera::getTarget() const { return this->target; }
