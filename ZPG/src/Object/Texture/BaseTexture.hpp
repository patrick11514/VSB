#pragma once

/*
 * File: BaseTexture.hpp
 * Author: Patrik Mintěl (MIN0150)
 * Description: File contains BaseTexture class
 */

// Include GLEW
#include <GL/glew.h>
// Include GLFW
#include <GLFW/glfw3.h>
#include <array>
#include <string>

/**
 * @brief Base Texture class, which only contains raw texture
 */
class BaseTexture {
private:
  static GLuint currentUnit; ///< Current rendering unit, so we can assing and
                             ///< increse them for new texture
  GLuint textureId;          ///< Texture id assigned from soil
  GLuint textureUnit;        ///< Assigned texture unit

public:
  /**
   * @brief Basic BaseTexture constructor
   * @param path Path to texture
   * @param textureType type of texture (GL_TEXTURE_2D...)
   */
  BaseTexture(const char *path, GLuint textureType);
  /**
   * @brief BaseTexture constructor With path as std::string
   * @param path Path to texture
   * @param textureType type of texture (GL_TEXTURE_2D...)
   */
  BaseTexture(const std::string &path, GLuint textureType)
      : BaseTexture(path.c_str(), textureType) {};
  /**
   * @brief CubeMap BaseTexture constructor
   * @param faces Faces of cubemap
   * @param textureType type of texture (GL_TEXTURE_2D...)
   */
  BaseTexture(std::array<std::string, 6> faces, GLuint textureType);

  GLuint getUnit() const;
};
