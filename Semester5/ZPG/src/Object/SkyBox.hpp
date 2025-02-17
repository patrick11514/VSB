#pragma once

/*
 * File: SkyBox.hpp
 * Author: Patrik Mintěl (MIN0150)
 * Description: This file contains SkyBox class
 */

#include "../Patterns/Observable.hpp"
#include "../Shader/ShaderProgram.hpp"
#include "BaseObject.hpp"
#include "ObjectData.hpp"
#include "Texture/BaseTexture.hpp"

#include <array>
#include <glm/ext/vector_float3.hpp>
#include <memory>

/**
 * @brief SkyBox class
 */
class SkyBox : public BaseObject, public Observer, public BaseTexture {
private:
  glm::mat4 modelMatrix; ///< Model matrix of skybox, since we don't implement
                         ///< Drawable
  std::shared_ptr<ObjectData> data; ///< Object data
  ShaderProgram *shaderProgram;     ///< Shader program for drawing

public:
  /**
   * @brief SkyBox constructor
   * @param faces Array of 6 paths to textures for skybox (posx, negx, posy,
   * negy, posz, negz)
   * @param shaderProgram Shader Program to draw skybox
   */
  SkyBox(std::array<std::string, 6> faces, ShaderProgram *shaderProgram);

  void update(Observable *who);
  void assignScene(Scene *scene);
  void draw() const;
};
