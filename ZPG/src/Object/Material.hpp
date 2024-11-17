#pragma once

#include <glm/ext/vector_float3.hpp>
class Material {
private:
  glm::vec3 ra; // ambient color
  glm::vec3 rd; // diffuse color
  glm::vec3 rs; // shinines color

public:
  Material(glm::vec3 ra, glm::vec3 rd, glm::vec3 rs) : ra(ra), rd(rd), rs(rs) {}

  glm::vec3 getRa() const;
  glm::vec3 getRd() const;
  glm::vec3 getRs() const;
};
