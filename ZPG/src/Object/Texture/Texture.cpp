#include "Texture.hpp"

#include <soil2/SOIL2.h>
#include <stdexcept>

GLuint Texture::currentUnit = GL_TEXTURE0;

Texture::Texture(glm::vec3 ra, glm::vec3 rd, glm::vec3 rs, float shininess,
                 const char *path, GLuint textureType)
    : Material(ra, rd, rs, shininess) {

  this->textureId = SOIL_load_OGL_texture(
      path, SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);

  if (this->textureId == static_cast<GLuint>(NULL)) {
    throw std::runtime_error("An error occurred while loading texture.");
  }

  this->textureUnit = this->currentUnit++;
  glActiveTexture(this->textureUnit);

  glBindTexture(textureType, this->textureId);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                  GL_REPEAT); // Horizontal (S axis)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                  GL_REPEAT); // Vertical (T axis)
}

GLuint Texture::getUnit() const { return this->textureUnit; }
