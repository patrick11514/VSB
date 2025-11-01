#include "raytracer.hpp"
#include "../objloader/objloader.hpp"
#include "../utils/triangle.hpp"
#include "../utils/utils.hpp"
#include "../utils/math.hpp"
#include <stdexcept>
#include <imgui.h>

Raytracer::Raytracer(const int width, const int height, const float fov_y,
                     const glm::vec3 view_from, const glm::vec3 view_at,
                     const char *config)
    : SimpleGuiSDL3(width, height), spherical_map_("../data/brown_photostudio_02_4k.exr")
{
  InitDeviceAndScene(config);

  camera_ = Camera(width, height, fov_y, view_from, view_at);
  lights_.emplace_back(new Light(glm::vec3{100.f, 150.f, 200.f}, .20f,
                                 glm::vec3{1.f, 1.f, 1.f}));
}

Raytracer::~Raytracer()
{
  ReleaseDeviceAndScene();

  for (auto *light : lights_)
  {
    delete light;
  }
}

int Raytracer::InitDeviceAndScene(const char *config)
{
  device_ = rtcNewDevice(config);
  error_handler(nullptr, rtcGetDeviceError(device_),
                "Unable to create a new device.\n");
  rtcSetDeviceErrorFunction(device_, error_handler, nullptr);

  ssize_t triangle_supported = rtcGetDeviceProperty(
      device_, RTC_DEVICE_PROPERTY_TRIANGLE_GEOMETRY_SUPPORTED);

  // create a new scene bound to the specified device
  scene_ = rtcNewScene(device_);

  return S_OK;
}

int Raytracer::ReleaseDeviceAndScene()
{
  rtcReleaseScene(scene_);
  rtcReleaseDevice(device_);

  return S_OK;
}

void Raytracer::LoadScene(const std::string file_name)
{
  const int no_surfaces = LoadOBJ(file_name.c_str(), surfaces_, materials_);

  // surfaces loop
  for (auto surface : surfaces_)
  {
    RTCGeometry mesh = rtcNewGeometry(device_, RTC_GEOMETRY_TYPE_TRIANGLE);

    SimpleVec3f *vertices = (SimpleVec3f *)rtcSetNewGeometryBuffer(
        mesh, RTC_BUFFER_TYPE_VERTEX, 0, RTC_FORMAT_FLOAT3, sizeof(SimpleVec3f),
        3 * surface->no_triangles());

    Triangle3ui *triangles = (Triangle3ui *)rtcSetNewGeometryBuffer(
        mesh, RTC_BUFFER_TYPE_INDEX, 0, RTC_FORMAT_UINT3, sizeof(Triangle3ui),
        surface->no_triangles());

    rtcSetGeometryUserData(mesh, (void *)(surface->get_material()));

    rtcSetGeometryVertexAttributeCount(mesh, 2);

    SimpleVec3f *normals = (SimpleVec3f *)rtcSetNewGeometryBuffer(
        mesh, RTC_BUFFER_TYPE_VERTEX_ATTRIBUTE, 0, RTC_FORMAT_FLOAT3,
        sizeof(SimpleVec3f), 3 * surface->no_triangles());

    Coord2f *tex_coords = (Coord2f *)rtcSetNewGeometryBuffer(
        mesh, RTC_BUFFER_TYPE_VERTEX_ATTRIBUTE, 1, RTC_FORMAT_FLOAT2,
        sizeof(Coord2f), 3 * surface->no_triangles());

    // triangles loop
    for (int i = 0, k = 0; i < surface->no_triangles(); ++i)
    {
      Triangle &triangle = surface->get_triangle(i);

      // vertices loop
      for (int j = 0; j < 3; ++j, ++k)
      {
        const Vertex &vertex = triangle.vertex(j);

        vertices[k].x = vertex.position.x;
        vertices[k].y = vertex.position.y;
        vertices[k].z = vertex.position.z;

        normals[k].x = vertex.normal.x;
        normals[k].y = vertex.normal.y;
        normals[k].z = vertex.normal.z;

        tex_coords[k].u = vertex.texture_coords[0].u;
        tex_coords[k].v = vertex.texture_coords[0].v;
      } // end of vertices loop

      triangles[i].v0 = k - 3;
      triangles[i].v1 = k - 2;
      triangles[i].v2 = k - 1;
    } // end of triangles loop

    rtcCommitGeometry(mesh);
    unsigned int geom_id = rtcAttachGeometry(scene_, mesh);
    rtcReleaseGeometry(mesh);
  } // end of surfaces loop

  rtcCommitScene(scene_);
}

Color4f Raytracer::get_pixel(const int x, const int y, const float t)
{

  RTCRayHit ray_hit{};
  ray_hit.ray = camera_.GenerateRay((float)x, (float)y);
  ray_hit.hit.geomID = RTC_INVALID_GEOMETRY_ID;

  auto color = Trace(ray_hit, 0);
  return Color4f{color.x, color.y, color.z, 1.f};
}

RTCRay Raytracer::GenerateNextRay(const glm::vec3 &position, const glm::vec3 &dir, float ior)
{
  RTCRay out{};
  out.org_x = position.x + EPSILON;
  out.org_y = position.y + EPSILON;
  out.org_z = position.z + EPSILON;

  out.dir_x = dir.x;
  out.dir_y = dir.y;
  out.dir_z = dir.z;

  out.tnear = 0.01;
  out.tfar = FLT_MAX;
  out.time = ior;
  out.mask = 0;
  out.flags = 0;

  return out;
}

glm::vec3 Raytracer::DirectDiffuse(const glm::vec3 &position, const glm::vec3 &normal)
{
  glm::vec3 color{.0f, .0f, .0f};

  for (auto *L : lights_)
  {
    RTCRay shadowRay{};
    glm::vec3 lightDir = L->position - position;
    float lightDistance = glm::length(lightDir);
    lightDir = glm::normalize(lightDir);

    shadowRay.org_x = position.x + EPSILON;
    shadowRay.org_y = position.y + EPSILON;
    shadowRay.org_z = position.z + EPSILON;

    shadowRay.dir_x = lightDir.x;
    shadowRay.dir_y = lightDir.y;
    shadowRay.dir_z = lightDir.z;

    shadowRay.tnear = 0.01;
    shadowRay.tfar = lightDistance;
    shadowRay.time = 0;

    shadowRay.mask = 0;
    shadowRay.id = 0;
    shadowRay.flags = 0;

    RTCRayHit shadowRayHit{};
    shadowRayHit.ray = shadowRay;
    shadowRayHit.hit.geomID = RTC_INVALID_GEOMETRY_ID;

    RTCIntersectContext context;
    rtcInitIntersectContext(&context);
    rtcIntersect1(scene_, &context, &shadowRayHit);

    if (shadowRayHit.hit.geomID == RTC_INVALID_GEOMETRY_ID)
    {
      // We are not in shadow YAYYYYY
      auto dot = glm::dot(lightDir, normal);
      if (dot > 0.f)
      {
        color += L->intensity * L->color * dot;
      }
    }
  }

  return color;
}

glm::vec3 Raytracer::Trace(RTCRayHit &ray_hit, int depth, int max_depth)
{
  if (depth < max_depth)
  {
    RTCIntersectContext context;
    rtcInitIntersectContext(&context);
    rtcIntersect1(scene_, &context, &ray_hit);

    if (ray_hit.hit.geomID != RTC_INVALID_GEOMETRY_ID)
    {

      auto geometry = rtcGetGeometry(scene_, ray_hit.hit.geomID);

      Normal3f normal;
      // get interpolated normal
      rtcInterpolate0(geometry, ray_hit.hit.primID, ray_hit.hit.u, ray_hit.hit.v,
                      RTC_BUFFER_TYPE_VERTEX_ATTRIBUTE, 0, &normal.x, 3);
      // and texture coordinates
      Coord2f tex_coord;
      rtcInterpolate0(geometry, ray_hit.hit.primID, ray_hit.hit.u, ray_hit.hit.v,
                      RTC_BUFFER_TYPE_VERTEX_ATTRIBUTE, 1, &tex_coord.u, 2);

      tex_coord.v = 1 - tex_coord.v;

      auto material = (Material *)rtcGetGeometryUserData(geometry);

      glm::vec3 O{ray_hit.ray.org_x, ray_hit.ray.org_y, ray_hit.ray.org_z};
      glm::vec3 D{ray_hit.ray.dir_x, ray_hit.ray.dir_y, ray_hit.ray.dir_z};
      float t = ray_hit.ray.tfar;
      glm::vec3 P = O + D * t;

      glm::vec3 N = glm::normalize(glm::vec3{normal.x, normal.y, normal.z});
      auto frontFace = glm::dot(D, N) < 0.f;
      if (!frontFace)
        N = -N;

      if (material->shader == Shader::BASIC)
      {
        auto color = material->diffuse;

        auto tex_diffuse = material->get_texture(Material::kDiffuseMapSlot);
        if (tex_diffuse)
        {
          Color3f texel = tex_diffuse->get_texel(tex_coord.u, tex_coord.v);
          color.x *= texel.r;
          color.y *= texel.g;
          color.z *= texel.b;
        }
        return color * DirectDiffuse(P, N);
      }

      auto reflRefr = ReflectRefract(ReflectRefractData{
          .normal = N,
          .direction = D,
          .position = P,
          .hit = ray_hit,
          .material = material,
          .depth = depth + 1,
          .max_depth = max_depth,
      });

      return reflRefr;
    }
  }

  return spherical_map_.texel(ray_hit.ray.dir_x, ray_hit.ray.dir_y, ray_hit.ray.dir_z);
}

glm::vec3 Raytracer::ReflectRefract(const ReflectRefractData &data)
{
  auto reflected = glm::reflect(glm::normalize(data.direction), data.normal);

  auto n1 = data.hit.ray.time; // here we have ior stored
  auto n2 = data.material->ior;

  if (n1 == n2)
  {
    // we left the object
    n2 = AIR_INDICIE;
  }

  float n1n2 = n1 / n2;
  float cosI = -glm::dot(glm::normalize(data.direction), data.normal);

  float sin2T = n1n2 * n1n2 * (1.0f - cosI * cosI);
  float cosT = std::sqrtf(1.f - sin2T);

  bool TIR = sin2T > 1.0f;
  float R;
  if (TIR)
  {
    R = 1.0f;
  }
  else
  {
    float F0 = (n1 - n2) / (n1 + n2);
    F0 *= F0;
    float cosTheta = n1 <= n2 ? cosI : cosT;
    R = F0 + (1 - F0) * (std::powf(1.f - cosTheta, 5.f));
  }

  auto dn = glm::dot(data.direction, data.normal);
  auto sqr = 1 - n1n2 * n1n2 * (1 - dn * dn);

  auto refracted = n1n2 * data.direction - (n1n2 * dn + std::sqrtf(sqr)) * data.normal;

  auto reflectedRay = CreateEmptyRayHit(GenerateNextRay(data.position, reflected, n1));
  if (R == 1.0f)
  {
    return Trace(reflectedRay, data.depth, data.max_depth);
  }
  auto refractedRay = CreateEmptyRayHit(GenerateNextRay(data.position, refracted, n2));

  auto refl = Trace(reflectedRay, data.depth, data.max_depth);
  auto refr = Trace(refractedRay, data.depth, data.max_depth);

  glm::vec3 tint = data.material->attenuation;
  refr *= tint;

  return R * refl + (1 - R) * refr;
}

int Raytracer::Ui()
{
  static int counter = 0;

  // Use a Begin/End pair to created a named window
  ImGui::Begin("Ray Tracer Params");

  ImGui::Text("Surfaces = %d", surfaces_.size());
  ImGui::Text("Materials = %d", materials_.size());
  ImGui::Separator();

  // ImGui::Checkbox( "Demo Window", &show_demo_window ); // Edit bools storing
  // our window open/close state ImGui::Checkbox( "Another Window",
  // &show_another_window );

  static float camera_x = 175.f; // 175, -140, 130
  static float camera_y = -140.f;
  static float camera_z = 130.f;

  float prev_camera_x = camera_x;
  float prev_camera_y = camera_y;
  float prev_camera_z = camera_z;

  ImGui::SliderFloat("Camera X", &camera_x, 0.0f, 1000.0f);
  ImGui::SliderFloat("Camera Y", &camera_y, 0.0f, 1000.0f);
  ImGui::SliderFloat("Camera Z", &camera_z, 0.0f, 1000.0f);

  if (prev_camera_x != camera_x || prev_camera_y != camera_y || prev_camera_z != camera_z)
  {
    // std::lock_guard<std::mutex> lock(temp_buffer_lock_);
    camera_.set_view_from(glm::vec3{camera_x, camera_y, camera_z});
  }

  // Buttons return true when clicked (most widgets return true when
  // edited/activated)
  if (ImGui::Button("Button"))
    counter++;
  ImGui::SameLine();
  ImGui::Text("counter = %d", counter);

  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
              1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
  ImGui::Text("Renderer FPS: %.1f", producer_fps_.load());
  ImGui::End();

  // 3. Show another simple window.
  /*if ( show_another_window )
  {
  ImGui::Begin( "Another Window", &show_another_window ); // Pass a pointer to
  our bool variable (the window will have a closing button that will clear the
  bool when clicked) ImGui::Text( "Hello from another window!" ); if (
  ImGui::Button( "Close Me" ) ) show_another_window = false; ImGui::End();
  }*/

  return 0;
}