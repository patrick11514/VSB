#include "surface.hpp"

Surface *BuildSurface(const std::string &name,
                      std::vector<Vertex> &face_vertices) {
  const size_t no_vertices = face_vertices.size();

  assert((no_vertices > 0) && (no_vertices % 3 == 0));

  const size_t no_triangles = no_vertices / 3;

  Surface *surface = new Surface(name, no_triangles);

  // kop�rov�n� dat
  for (size_t i = 0; i < no_triangles; ++i) {
    surface->get_triangles()[i] =
        Triangle(face_vertices[i * 3], face_vertices[i * 3 + 1],
                 face_vertices[i * 3 + 2], surface);
  }

  return surface;
}

Surface::Surface() {
  n_ = 0;
  triangles_ = NULL;
}

Surface::Surface(const std::string &name, const size_t n) {
  assert(n > 0);

  name_ = name;

  n_ = n;
  triangles_ = new Triangle[n_];
}

Surface::~Surface() {
  if (triangles_) {
    delete[] triangles_;
    triangles_ = nullptr;
  }
  n_ = 0;
}

Triangle &Surface::get_triangle(const size_t i) { return triangles_[i]; }

Triangle *Surface::get_triangles() { return triangles_; }

std::string Surface::get_name() { return name_; }

size_t Surface::no_triangles() { return n_; }

size_t Surface::no_vertices() { return 3 * n_; }

void Surface::set_material(Material *material) { material_ = material; }

Material *Surface::get_material() const { return material_; }
