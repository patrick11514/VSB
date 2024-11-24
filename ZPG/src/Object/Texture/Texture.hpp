#pragma once

// Include GLEW
#include <GL/glew.h>
// Include GLFW
#include <GLFW/glfw3.h>
#include <string>

#include "../Material/Material.hpp"

class Texture : public Material {
private:
  static GLuint currentUnit;
  GLuint textureId;
  GLuint textureUnit;

public:
  Texture(glm::vec3 ra, glm::vec3 rd, glm::vec3 rs, float shininess,
          const char *path, GLuint textureType);
  Texture(const Material &material, const char *path, GLuint textureType)
      : Texture(material.getRa(), material.getRd(), material.getRs(),
                material.getShininess(), path, textureType) {};
  Texture(glm::vec3 ra, glm::vec3 rd, glm::vec3 rs, float shininess,
          const std::string &path, GLuint textureType)
      : Texture(ra, rd, rs, shininess, path.c_str(), textureType) {};

  GLuint getUnit() const;
};
