#pragma once

// Include GLEW
#include <GL/glew.h>
// Include GLFW
#include <GLFW/glfw3.h>
#include <array>
#include <string>

class BaseTexture {
private:
  static GLuint currentUnit;
  GLuint textureId;
  GLuint textureUnit;

public:
  BaseTexture(const char *path, GLuint textureType);
  BaseTexture(const std::string &path, GLuint textureType)
      : BaseTexture(path.c_str(), textureType) {};
  BaseTexture(std::array<std::string, 6> faces, GLuint textureType);

  GLuint getUnit() const;
};
