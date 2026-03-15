#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <vector>

// Forward declarations
class ShaderProgram;
class Camera;
class Controller;

#include "system/ISystem.hpp"

struct GPUMaterial {
  alignas(16) glm::vec4 ambient;  // w = padding
  alignas(16) glm::vec4 diffuse;  // w = padding
  alignas(16) glm::vec4 specular; // w = shininess
  alignas(16) glm::vec4
      pbrTextureTypes; // x=albedo, y=normal, z=metallic, w=roughness (0 =
                       // color/default, 1 = texture)
  alignas(16) glm::vec4
      pbrTextureIndices; // x=albedo, y=normal, z=metallic, w=roughness
  alignas(16) glm::vec4 pbrTextureTypes2;   // x=ao (0 = default, 1 = texture)
  alignas(16) glm::vec4 pbrTextureIndices2; // x=ao
};

struct Mesh {
  GLuint vao;
  GLuint vbo;
  GLuint ebo;
  int indexCount;
  int materialIndex;
};

struct SceneData {
  std::vector<Mesh> meshes;
  std::vector<GPUMaterial> materials;
  std::vector<GLuint> textureIds;
};

class Rasterizer {
private:
  int width;
  int height;
  const char *title;

  GLFWwindow *window;
  Camera *camera;
  Controller *controller;

  ShaderProgram *program;

  SceneData scene;
  GLuint materialSSBO;
  entt::registry registry;
  std::vector<std::unique_ptr<ISystem>> systems;

  GLuint irradianceMap = 0;
  GLuint prefilteredMap = 0;
  GLuint brdfLUTMap = 0;

  friend class RenderSystem;

public:
  bool uiMode = false;
  bool showAxes = false;

private:
  void DrawUI();

  static void error_callback(int error, const char *description);

public:
  Rasterizer(int width, int height, const char *title);
  ~Rasterizer();

  void resize(int w, int h) {
    width = w;
    height = h;
    glViewport(0, 0, width, height);
  }

  void InitDevice();
  void InitPrograms();
  void LoadScene(const std::string &fileName);
  void CreateAxes();
  void InitBuffers();
  void InitMaterials(int bindingPoint);
  void InitIBLTextures();
  void LoadEXRTexture(const char *filepath, GLuint &texID, bool isSrgb = false,
                      bool isMipmap = false);
  void LoadPrefilteredEnvMap(const std::vector<std::string> &filepaths,
                             GLuint &texID);
  void MainLoop();
};
