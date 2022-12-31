#ifndef FILES_H
#define FILES_H 1

// default includes
#include <stdio.h>

// sdl
#include <SDL2/SDL.h>

// my includes
#include "breakout.h"

// Open file called _fileName with mode _mode
FILE *openFile(char *_fileName, char *_mode);

// load level from give path
Level *loadLevel(WindowProperties *WindowProperties, char *fileName);

// free level
void freeLevels(Array *levels);

#endif