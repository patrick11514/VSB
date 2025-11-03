#include "raytracer/raytracer.hpp"
#include "utils/math.hpp"
#include <cstdio>
#include <emmintrin.h>
#include <xmmintrin.h>

int main()
{
  printf("PG1, (c)2011-2025 Tomas Fabian\n\n");

  Raytracer raytracer(640, 480, deg2rad(45.0), glm::vec3(3, 0, 0),
                      glm::vec3(0, 0, 0), "threads=0,verbose=0");
  raytracer.LoadScene("../data/6887_allied_avenger.obj");
  raytracer.MainLoop();
}