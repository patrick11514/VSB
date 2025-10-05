#pragma once

#include <embree3/rtcore.h>
#include <glm/glm.hpp>

class Camera {
public:
  Camera() {}

  Camera(const int width, const int height, const float fov_y,
         const glm::vec3 view_from, const glm::vec3 view_at);

  /* generate primary ray, top-left pixel image coordinates (xi, yi) are in the
   * range <0, 1) x <0, 1) */
  RTCRay GenerateRay(const float xi, const float yi) const;

  void set_view_from(glm::vec3 new_view_from);

private:
  int width_{640};      // image width (px)
  int height_{480};     // image height (px)
  float fov_y_{0.785f}; // vertical field of view (rad)

  glm::vec3 view_from_;                       // ray origin or eye or O
  glm::vec3 view_at_;                         // target T
  glm::vec3 up_{glm::vec3{0.0f, 0.0f, 1.0f}}; // up vector

  float f_y_{1.0f}; // focal lenght (px)

  glm::mat3 M_c_w_; // transformation matrix from CS -> WS
};