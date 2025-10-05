#pragma once

#include "../camera/camera.hpp"
#include "../light/light.hpp"
#include "../ui/simpleguidx11_linux.hpp"
#include "../utils/structs.hpp"
#include "../utils/surface.hpp"

/*! \class Raytracer
\brief General ray tracer class.

\author Tom� Fabi�n
\version 0.1
\date 2018
*/
class Raytracer : public SimpleGuiLinux {
public:
  Raytracer(const int width, const int height, const float fov_y,
            const glm::vec3 view_from, const glm::vec3 view_at,
            const char *config = "threads=0,verbose=3");
  ~Raytracer();

  int InitDeviceAndScene(const char *config);

  int ReleaseDeviceAndScene();

  void LoadScene(const std::string file_name);

  Color4f get_pixel(const int x, const int y, const float t = 0.0f) override;
  RTCRay GenerateNextRay(RTCRayHit &ray_hit, glm::vec3 &normal);
  glm::vec3 Trace(RTCRayHit &ray_hit, int depth, int max_depth = 5);
  glm::vec3 DirectDiffuse(const glm::vec3 &P, const glm::vec3 &N);

  int Ui();

private:
  std::vector<Surface *> surfaces_;
  std::vector<Material *> materials_;
  std::vector<Light *> lights_;

  RTCDevice device_;
  RTCScene scene_;
  Camera camera_;
};