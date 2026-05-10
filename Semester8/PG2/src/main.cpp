#include "Rasterizer.hpp"
#include <glm/trigonometric.hpp>

/*
 * File: main.hpp
 * Author: Patrik Mintěl (MIN0150)
 * Description: The main file :)
 */

// Task 1-8: entry point that initializes the full PG2 rendering pipeline.
int main(void) {

  Rasterizer rasterizer(800, 600, "PG2 Framework");
  rasterizer.InitDevice();
  rasterizer.InitPrograms();
  rasterizer.LoadScene(/* */
                       "../models/6887_allied_avenger_gi2.obj"
                       //"../models/piece_02.obj"
                       //"../models/shadow_volume_test.obj"
                       //"../models/scene6.obj"
                       /* */);
  rasterizer.CreateAxes();
  rasterizer.InitBuffers();
  rasterizer.InitMaterials(0);
  rasterizer.InitIBLTextures();
  rasterizer.InitShadowMap();
  rasterizer.InitSkySphere();
  rasterizer.MainLoop();

  return EXIT_SUCCESS;
}