#include "camera.hpp"
#include <cstdio>

Camera::Camera(const int width, const int height, const float fov_y,
               const glm::vec3 view_from, const glm::vec3 view_at) {
  width_ = width;
  height_ = height;
  fov_y_ = fov_y;

  view_from_ = view_from;
  view_at_ = view_at;

  f_y_ = height_ / (2.f * std::tanf((fov_y_ / 2.f)));

  auto z_c = glm::normalize(view_from_ - view_at_);
  auto x_c = glm::normalize(glm::cross(up_, z_c));
  auto y_c = glm::normalize(glm::cross(z_c, x_c));

  M_c_w_ = glm::mat3(x_c, y_c, z_c);
}

RTCRay Camera::GenerateRay(const float x_i, const float y_i) const {
  RTCRay ray{};

  // TODO fill in ray structure and compute ray direction
  ray.org_x = view_from_.x;
  ray.org_y = view_from_.y;
  ray.org_z = view_from_.z;

  glm::vec3 d_c =
      glm::normalize(glm::vec3(x_i - (width_ / 2), (height_ / 2) - y_i, -f_y_));
  glm::vec3 d_w = glm::normalize(M_c_w_ * d_c);

  ray.dir_x = d_w.x;
  ray.dir_y = d_w.y;
  ray.dir_z = d_w.z;

  ray.tnear = FLT_MIN; // start of ray segment
  ray.tfar = FLT_MAX;  // end of ray segment (set to hit distance)
  ray.time = 0;

  ray.mask = 0;  // can be used to mask out some geometries for some rays
  ray.id = 0;    // identify a ray inside a callback function
  ray.flags = 0; // reserved

  return ray;
}

void Camera::set_view_from(glm::vec3 view_from) {
  view_from_ = view_from;

  // TODO compute focal lenght based on the vertical field of view and the
  // camera resolution
  f_y_ = height_ / (2.f * std::tanf((fov_y_ / 2.f)));

  auto z_c = glm::normalize(view_from_ - view_at_);
  auto x_c = glm::normalize(glm::cross(up_, z_c));

  auto y_c = glm::normalize(glm::cross(z_c, x_c));

  M_c_w_ = glm::mat3(x_c, y_c, z_c);
}