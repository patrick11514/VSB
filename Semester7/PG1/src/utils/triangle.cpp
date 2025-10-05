#include "triangle.hpp"

Triangle::Triangle(const Vertex &v0, const Vertex &v1, const Vertex &v2,
                   Surface *surface) {
  vertices_[0] = v0;
  vertices_[1] = v1;
  vertices_[2] = v2;

  assert(!is_degenerate());
}

Vertex Triangle::vertex(const int i) { return vertices_[i]; }

bool Triangle::is_degenerate() const {
  return vertices_[0].position == vertices_[1].position ||
         vertices_[0].position == vertices_[2].position ||
         vertices_[1].position == vertices_[2].position;
}
