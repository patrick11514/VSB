#pragma once

#include <GL/glew.h>
#include <entt/entt.hpp>
#include <glm/glm.hpp>

namespace attributes {
struct Name {
  std::string name;
};

struct Transform {
  glm::vec3 pos, rot, scale;
  bool useMatrix = false;
  glm::mat4 modelMatrix = glm::mat4(1.0f);
};

struct RenderMesh {
  std::vector<int> meshIndices;
};

struct CameraSync {
  glm::mat4 relativeMatrix = glm::mat4(1.0f);
};

struct Togglable {
  bool visible = true;
};

struct RenderOnTop {};

struct IsAxis {};

struct Parent {
  entt::entity entity;
};

struct Children {
  std::vector<entt::entity> entities;
};

struct Mesh {
  GLuint vao;
  GLuint vbo;
  GLuint ebo;
  int indexCount;
  int materialIndex;
};
}; // namespace attributes