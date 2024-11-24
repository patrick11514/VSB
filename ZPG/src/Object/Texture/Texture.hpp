#pragma once

// Include GLEW
#include <GL/glew.h>
// Include GLFW
#include <GLFW/glfw3.h>
#include <string>

#include "../Material/Material.hpp"
#include "BaseTexture.hpp"

class Texture : public Material, public BaseTexture {
public:
  Texture(glm::vec3 ra, glm::vec3 rd, glm::vec3 rs, float shininess,
          const char *path, GLuint textureType)
      : Material(ra, rd, rs, shininess), BaseTexture(path, textureType) {};
  Texture(const Material &material, const char *path, GLuint textureType)
      : Material(material), BaseTexture(path, textureType) {};
  Texture(glm::vec3 ra, glm::vec3 rd, glm::vec3 rs, float shininess,
          const std::string &path, GLuint textureType)
      : Material(ra, rd, rs, shininess), BaseTexture(path, textureType) {};
};
