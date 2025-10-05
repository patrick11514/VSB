#include "raytracer/raytracer.hpp"
#include "utils/math.hpp"
#include <cstdio>
#include <xmmintrin.h>

int main() {
  printf("PG1, (c)2011-2025 Tomas Fabian\n\n");

  _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
  _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_ON);

  Raytracer raytracer(640, 480, deg2rad(45.0), glm::vec3(175, -140, 130),
                      glm::vec3(0, 0, 35), "threads=0,verbose=0");
  raytracer.LoadScene("../data/6887_allied_avenger.obj");
  raytracer.MainLoop();
}