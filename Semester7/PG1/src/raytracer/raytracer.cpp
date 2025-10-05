#include "raytracer.hpp"
#include "../objloader/objloader.hpp"
#include "../utils/utils.hpp"

Raytracer::Raytracer(const int width, const int height, const float fov_y,
                     const glm::vec3 view_from, const glm::vec3 view_at,
                     const char *config)
    : SimpleGuiLinux(width, height) {
  InitDeviceAndScene(config);

  camera_ = Camera(width, height, fov_y, view_from, view_at);
  lights_.emplace_back(new Light(glm::vec3{100.f, 150.f, 200.f}, .20f,
                                 glm::vec3{1.f, 1.f, 1.f}));
}

Raytracer::~Raytracer() {
  ReleaseDeviceAndScene();

  for (auto *light : lights_) {
    delete light;
  }
}

int Raytracer::InitDeviceAndScene(const char *config) {
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

int Raytracer::ReleaseDeviceAndScene() {
  rtcReleaseScene(scene_);
  rtcReleaseDevice(device_);

  return S_OK;
}

void Raytracer::LoadScene(const std::string file_name) {
  const int no_surfaces = LoadOBJ(file_name.c_str(), surfaces_, materials_);

  // surfaces loop
  for (auto surface : surfaces_) {
    RTCGeometry mesh = rtcNewGeometry(device_, RTC_GEOMETRY_TYPE_TRIANGLE);

    glm::vec3 *vertices = (glm::vec3 *)rtcSetNewGeometryBuffer(
        mesh, RTC_BUFFER_TYPE_VERTEX, 0, RTC_FORMAT_FLOAT3, sizeof(glm::vec3),
        3 * surface->no_triangles());

    Triangle3ui *triangles = (Triangle3ui *)rtcSetNewGeometryBuffer(
        mesh, RTC_BUFFER_TYPE_INDEX, 0, RTC_FORMAT_UINT3, sizeof(Triangle3ui),
        surface->no_triangles());

    rtcSetGeometryUserData(mesh, (void *)(surface->get_material()));

    rtcSetGeometryVertexAttributeCount(mesh, 2);

    Normal3f *normals = (Normal3f *)rtcSetNewGeometryBuffer(
        mesh, RTC_BUFFER_TYPE_VERTEX_ATTRIBUTE, 0, RTC_FORMAT_FLOAT3,
        sizeof(Normal3f), 3 * surface->no_triangles());

    Coord2f *tex_coords = (Coord2f *)rtcSetNewGeometryBuffer(
        mesh, RTC_BUFFER_TYPE_VERTEX_ATTRIBUTE, 1, RTC_FORMAT_FLOAT2,
        sizeof(Coord2f), 3 * surface->no_triangles());

    // triangles loop
    for (int i = 0, k = 0; i < surface->no_triangles(); ++i) {
      Triangle &triangle = surface->get_triangle(i);

      // vertices loop
      for (int j = 0; j < 3; ++j, ++k) {
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

Color4f Raytracer::get_pixel(const int x, const int y, const float t) {

  RTCRayHit ray_hit{};
  ray_hit.ray = camera_.GenerateRay((float)x, (float)y);
  ray_hit.hit.geomID = RTC_INVALID_GEOMETRY_ID;

  auto color = Trace(ray_hit, 0);
  return Color4f{color.x, color.y, color.z, 1.f};
}

RTCRay Raytracer::GenerateNextRay(RTCRayHit &hit, glm::vec3 &_normal) {
  glm::vec3 origin{hit.ray.org_x, hit.ray.org_y, hit.ray.org_z};
  glm::vec3 direction =
      glm::normalize(glm::vec3{hit.ray.dir_x, hit.ray.dir_y, hit.ray.dir_z});
  float t = hit.ray.tfar;
  glm::vec3 point = origin + direction * t;

  auto normal = glm::normalize(_normal);
  if (glm::dot(direction, normal) > 0.0f)
    normal = -normal;

  glm::vec3 reflected =
      glm::normalize(normal * (2.0f * glm::dot(direction, normal)) - direction);

  const float eps = 1e-4f;

  RTCRay out{};
  out.org_x = point.x + normal.x * eps;
  out.org_y = point.y + normal.y * eps;
  out.org_z = point.z + normal.z * eps;

  out.dir_x = reflected.x;
  out.dir_y = reflected.y;
  out.dir_z = reflected.z;

  out.tnear = 0.0f;
  out.tfar = FLT_MAX;
  out.time = 0.0f;
  out.mask = -1;
  out.flags = 0;

  return out;
}

glm::vec3 Raytracer::DirectDiffuse(const glm::vec3 &position,
                                   const glm::vec3 &normal) {
  const float eps = 1e-4f;
  glm::vec3 Lo{0.f, 0.f, 0.f};

  for (auto *L : lights_) {
    glm::vec3 Lvec = L->position - position;
    float r2 = glm::dot(Lvec, Lvec);
    float r = std::sqrt(r2);
    glm::vec3 Ldir = Lvec / r;

    RTCRayHit sh{};
    sh.ray.org_x = position.x + normal.x * eps;
    sh.ray.org_y = position.y + normal.y * eps;
    sh.ray.org_z = position.z + normal.z * eps;
    sh.ray.dir_x = Ldir.x;
    sh.ray.dir_y = Ldir.y;
    sh.ray.dir_z = Ldir.z;
    sh.ray.tnear = 0.0f;
    sh.ray.tfar = r - eps;
    sh.ray.mask = -1;
    sh.ray.time = 0.0f;
    sh.ray.flags = 0;
    sh.hit.geomID = RTC_INVALID_GEOMETRY_ID;

    RTCIntersectContext ctx;
    rtcInitIntersectContext(&ctx);
    rtcIntersect1(scene_, &ctx, &sh);
    if (sh.hit.geomID != RTC_INVALID_GEOMETRY_ID)
      continue;

    float NdotL = glm::dot(normal, Ldir);
    if (NdotL < 1.0) {
      NdotL = 1.0f;
    }
    Lo = Lo + L->radiance() * (NdotL / r2);
  }
  return Lo;
}

glm::vec3 Raytracer::Trace(RTCRayHit &ray_hit, int depth, int max_depth) {
  if (depth >= max_depth)
    return glm::vec3{1.f, 0.5f, 0.345f};

  RTCIntersectContext context;
  rtcInitIntersectContext(&context);
  rtcIntersect1(scene_, &context, &ray_hit);

  if (ray_hit.hit.geomID == RTC_INVALID_GEOMETRY_ID)
    return glm::vec3{1.f, 0.5f, 0.345f};

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
  auto color = material->diffuse;

  auto tex_diffuse = material->get_texture(Material::kDiffuseMapSlot);
  if (tex_diffuse) {
    Color3f texel = tex_diffuse->get_texel(tex_coord.u, tex_coord.v);
    color.x *= texel.r;
    color.y *= texel.g;
    color.z *= texel.b;
  }

  glm::vec3 O{ray_hit.ray.org_x, ray_hit.ray.org_y, ray_hit.ray.org_z};
  glm::vec3 D = glm::normalize(
      glm::vec3{ray_hit.ray.dir_x, ray_hit.ray.dir_y, ray_hit.ray.dir_z});
  float t = ray_hit.ray.tfar;
  glm::vec3 P = O + D * t;

  glm::vec3 N = glm::normalize(glm::vec3{normal.x, normal.y, normal.z});
  if (glm::dot(D, N) > 0.f)
    N = -N;

  if (material->shader == Shader::BASIC) {
    return color * DirectDiffuse(P, N);
  }

  RTCRayHit nextRayHit{};
  auto vNormal = glm::vec3{normal.x, normal.y, normal.z};
  nextRayHit.ray = GenerateNextRay(ray_hit, vNormal);
  nextRayHit.hit.geomID = RTC_INVALID_GEOMETRY_ID;
  return color * Trace(nextRayHit, depth + 1, max_depth);
}

int Raytracer::Ui() {
  static int counter = 0;

  // Use a Begin/End pair to created a named window
  ImGui::Begin("Ray Tracer Params");

  ImGui::Text("Surfaces = %d", surfaces_.size());
  ImGui::Text("Materials = %d", materials_.size());
  ImGui::Separator();
  ImGui::Checkbox("Vsync", &vsync_);

  // ImGui::Checkbox( "Demo Window", &show_demo_window ); // Edit bools storing
  // our window open/close state ImGui::Checkbox( "Another Window",
  // &show_another_window );

  static float camera_x = 175.f; // 175, -140, 130
  static float camera_y = -140.f;
  static float camera_z = 130.f;

  ImGui::SliderFloat("Camera X", &camera_x, 0.0f, 1000.0f);
  ImGui::SliderFloat("Camera Y", &camera_y, 0.0f, 1000.0f);
  ImGui::SliderFloat("Camera Z", &camera_z, 0.0f, 1000.0f);

  camera_.set_view_from(glm::vec3{camera_x, camera_y, camera_z});

  // Buttons return true when clicked (most widgets return true when
  // edited/activated)
  if (ImGui::Button("Button"))
    counter++;
  ImGui::SameLine();
  ImGui::Text("counter = %d", counter);

  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
              1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
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