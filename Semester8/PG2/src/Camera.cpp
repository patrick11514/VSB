#include "Camera.hpp"
#include <glm/ext/matrix_clip_space.hpp>

Camera::Camera(float fov, float zNear, float zFar)
    : fov(fov), zNear(zNear), zFar(zFar)
{
  this->recalculateTarget();
}

void Camera::recalculateTarget()
{
  double radPitch = glm::radians(this->pitch);
  double radYaw = glm::radians(this->yaw);

  this->target.x = std::cos(radPitch) * std::cos(radYaw);
  this->target.y = std::sin(radPitch);
  this->target.z = std::cos(radPitch) * std::sin(radYaw);

  this->target = glm::normalize(this->target);
  this->calculateViewMatrix();
}

void Camera::toLeft(float rate)
{
  this->eye -= glm::normalize(glm::cross(this->target, this->up)) * rate;
  this->calculateViewMatrix();
}

void Camera::toRight(float rate)
{
  this->eye += glm::normalize(glm::cross(this->target, this->up)) * rate;
  this->calculateViewMatrix();
}

void Camera::forward(float rate)
{
  glm::vec3 planarForward =
      glm::normalize(glm::vec3(this->target.x, 0.0f, this->target.z));
  this->eye += planarForward * rate;
  this->calculateViewMatrix();
}

void Camera::backward(float rate)
{
  glm::vec3 planarForward =
      glm::normalize(glm::vec3(this->target.x, 0.0f, this->target.z));
  this->eye -= planarForward * rate;
  this->calculateViewMatrix();
}

void Camera::upMovement(float rate)
{
  this->eye += this->up * rate;
  this->calculateViewMatrix();
}

void Camera::downMovement(float rate)
{
  this->eye -= this->up * rate;
  this->calculateViewMatrix();
}

void Camera::changeYaw(float deg)
{
  this->yaw += deg;
  this->recalculateTarget();
}

void Camera::changePitch(float deg)
{
  this->pitch += deg;

  if (this->pitch > 89.0f)
  {
    this->pitch = 89.0f;
  }
  if (this->pitch < -89.0f)
  {
    this->pitch = -89.0f;
  }
  this->recalculateTarget();
}

void Camera::calculateViewMatrix()
{

  this->viewMatrix = glm::lookAt(this->eye, this->eye + this->target, this->up);
}

glm::mat4 Camera::getViewMatrix() const { return this->viewMatrix; }

glm::vec3 Camera::getPosition() const { return this->eye; }

glm::vec3 Camera::getTarget() const { return this->target; }
