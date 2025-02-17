#pragma once

/*
 * File: Texture.hpp
 * Author: Patrik Mintěl (MIN0150)
 * Description: File contains Texture class
 */

// Include GLEW
#include <GL/glew.h>
// Include GLFW
#include <GLFW/glfw3.h>
#include <string>

#include "../Material/Material.hpp"
#include "BaseTexture.hpp"

/**
 * @brief Texture class, which implements also material into it, for setting ra,
 * rd, rs, shininess
 */
class Texture : public Material, public BaseTexture {
public:
  /**
   * @brief Same in BaseTexture, Basic Texture constructor
   * @param ra Ambient reflectivity
   * @param rd Diffuse reflectivity
   * @param rs Specular reflectivity
   * @param shininess Shininess coeficient
   * @param path Path to texture
   * @param textureType Type of texture
   */
  Texture(glm::vec3 ra, glm::vec3 rd, glm::vec3 rs, float shininess,
          const char *path, GLuint textureType)
      : Material(ra, rd, rs, shininess), BaseTexture(path, textureType) {};

  /**
   * @brief Same as Base Texture constructor, but can be constructed from
   * material, so we don't need to set ra,rd,rs...
   * @param material Material which will provide ra, rd, rs and shininess
   * @param path Path to texture
   * @param textureType Type of texture
   */
  Texture(const Material &material, const char *path, GLuint textureType)
      : Material(material), BaseTexture(path, textureType) {};
  /**
   * @brief Same in BaseTexture, Texture constructor from std::string path
   * @param ra Ambient reflectivity
   * @param rd Diffuse reflectivity
   * @param rs Specular reflectivity
   * @param shininess Shininess coeficient
   * @param path Path to texture
   * @param textureType Type of texture
   */
  Texture(glm::vec3 ra, glm::vec3 rd, glm::vec3 rs, float shininess,
          const std::string &path, GLuint textureType)
      : Material(ra, rd, rs, shininess), BaseTexture(path, textureType) {};
};
