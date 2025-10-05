#pragma once

#include <embree3/rtcore.h>
#include <glm/glm.hpp>

using Normal3f =
    glm::vec3; // a single vertex normal structure matching certain format

struct Coord2f {
  float u, v;
}; // texture coord structure

struct Triangle3ui {
  unsigned int v0, v1, v2;
}; // indicies of a single triangle, the struct must match certain format, e.g.
   // RTC_FORMAT_UINT3

struct RTC_ALIGN(16) Color4f {
  struct {
    float r, g, b, a;
  }; // a = 1 means that the pixel is opaque
};

struct Color3f {
  float r, g, b;
};

struct SimpleVec3f {
  float x, y, z;
};