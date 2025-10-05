#include "vertex.hpp"

Vertex::Vertex(const glm::vec3 position, const glm::vec3 normal,
               glm::vec3 color, Coord2f *texture_coords) {
  this->position = position;
  this->normal = normal;
  this->color = color;

  if (texture_coords != NULL) {
    for (int i = 0; i < NO_TEXTURE_COORDS; ++i) {
      this->texture_coords[i] = texture_coords[i];
    }
  }
}