#pragma once

#include "../camera/camera.hpp"
#include "../light/light.hpp"
#include "../ui/gui_linux.hpp"
#include "../utils/structs.hpp"
#include "../utils/surface.hpp"
#include "../spherical_map/spherical_map.hpp"

struct ReflectRefractData
{
  glm::vec3 normal;
  glm::vec3 direction;
  glm::vec3 position;
  RTCRayHit hit;
  Material *material;
  int depth;
  int max_depth;
};

class Raytracer : public SimpleGuiSDL3
{
public:
  Raytracer(const int width, const int height, const float fov_y,
            const glm::vec3 view_from, const glm::vec3 view_at,
            const char *config = "threads=0,verbose=3");
  ~Raytracer();

  int InitDeviceAndScene(const char *config);

  int ReleaseDeviceAndScene();

  void LoadScene(const std::string file_name);

  Color4f get_pixel(const int x, const int y, const float t = 0.0f) override;
  RTCRay GenerateNextRay(const glm::vec3 &position, const glm::vec3 &dir, float ior);
  glm::vec3 Trace(RTCRayHit &ray_hit, int depth, int max_depth = 10);
  glm::vec3 DirectDiffuse(const glm::vec3 &P, const glm::vec3 &N);
  glm::vec3 ReflectRefract(const ReflectRefractData &data);

  int Ui();

private:
  std::vector<Surface *> surfaces_;
  std::vector<Material *> materials_;
  std::vector<Light *> lights_;

  RTCDevice device_;
  RTCScene scene_;
  Camera camera_;
  SphericalMap spherical_map_;
};