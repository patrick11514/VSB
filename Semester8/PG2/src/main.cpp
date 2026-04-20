#include "Rasterizer.hpp"
#include <glm/trigonometric.hpp>

/*
 * File: main.hpp
 * Author: Patrik Mintěl (MIN0150)
 * Description: The main file :)
 */

int main(void)
{

  Rasterizer rasterizer(800, 600, "PG2 Framework");
  rasterizer.InitDevice();
  rasterizer.InitPrograms();
  rasterizer.LoadScene("../models/shadow_volume_test.obj");
  rasterizer.CreateAxes();
  rasterizer.InitBuffers();
  rasterizer.InitMaterials(0);
  rasterizer.InitIBLTextures();
  rasterizer.InitShadowMap();
  rasterizer.MainLoop();

  return EXIT_SUCCESS;
}