#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

// Forward declarations
class ShaderProgram;
class Camera;
class Controller;

struct GPUMaterial {
  alignas(16) glm::vec4 ambient;  // w = padding
  alignas(16) glm::vec4 diffuse;  // w = padding
  alignas(16) glm::vec4 specular; // w = shininess
  alignas(16) glm::vec4
      textureInfo; // x = type (0 = color, 1 = texture), y = textureIndex
};

struct Transform {
  glm::vec3 position{0.0f};
  glm::vec3 rotation{0.0f};
  glm::vec3 scale{1.0f};
};

struct RenderObject {
  std::string name;
  Transform transform;
  std::vector<int>
      meshIndices; // Points to indices in the SceneData.meshes array
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
  std::vector<RenderObject> objects;
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

public:
  bool uiMode = false;

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
  void MainLoop();
};
