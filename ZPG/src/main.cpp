#include "App.hpp"

/*
 * File: main.hpp
 * Author: Patrik Mintěl (MIN0150)
 * Description: The main file :)
 */

int main(void) {
  App app;
  app.initialize();
  app.prepareScenes();
  app.createShaders();
  app.createModels();
  app.run();
  app.destroy();

  return EXIT_SUCCESS;
}
