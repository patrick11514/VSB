#pragma once

/*
 * File: Material.hpp
 * Author: Patrik Mintěl (MIN0150)
 * Description: File contains basic Material class
 */

#include <glm/ext/vector_float3.hpp>
/**
 * @brief Material class
 */
class Material {
private:
  glm::vec3 ra;    // ambient color
  glm::vec3 rd;    // diffuse color
  glm::vec3 rs;    // shinines color
  float shininess; // shininess coeficient
public:
  /**
   * @brief Material constructor
   * @param ra Ambient reflectivity
   * @param rd Diffuse reflectivity
   * @param rs Specular reflectivity
   * @param shininess Shininess coeficient
   */
  Material(glm::vec3 ra, glm::vec3 rd, glm::vec3 rs, float shininess)
      : ra(ra), rd(rd), rs(rs), shininess(shininess) {}

  virtual ~Material() = default;

  glm::vec3 getRa() const;
  glm::vec3 getRd() const;
  glm::vec3 getRs() const;
  float getShininess() const;
};
