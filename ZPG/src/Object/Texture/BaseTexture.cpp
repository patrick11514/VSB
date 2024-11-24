#include "BaseTexture.hpp"

#include <soil2/SOIL2.h>
#include <stdexcept>

GLuint BaseTexture::currentUnit = GL_TEXTURE0;

BaseTexture::BaseTexture(const char *path, GLuint textureType) {
  this->textureId = SOIL_load_OGL_texture(
      path, SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);

  if (this->textureId == static_cast<GLuint>(NULL)) {
    throw std::runtime_error("An error occurred while loading texture.");
  }

  this->textureUnit = this->currentUnit++;

  printf("Unit: %d\n", this->textureUnit);

  glActiveTexture(this->textureUnit);

  glBindTexture(textureType, this->textureId);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                  GL_REPEAT); // Horizontal (S axis)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                  GL_REPEAT); // Vertical (T axis)
}

BaseTexture::BaseTexture(std::array<std::string, 6> paths, GLuint textureType) {
  printf("OK\n");
  this->textureId = SOIL_load_OGL_cubemap(
      paths[0].c_str(), paths[1].c_str(), paths[2].c_str(), paths[3].c_str(),
      paths[4].c_str(), paths[5].c_str(), SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID,
      SOIL_FLAG_MIPMAPS);

  if (this->textureId == static_cast<GLuint>(NULL)) {
    throw std::runtime_error("An error occurred while loading texture.");
  }

  this->textureUnit = this->currentUnit++;
  printf("Unit: %d\n", this->textureUnit);
  glActiveTexture(this->textureUnit);

  glBindTexture(textureType, this->textureId);
}

GLuint BaseTexture::getUnit() const { return this->textureUnit - GL_TEXTURE0; }
