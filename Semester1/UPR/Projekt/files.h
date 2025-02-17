#ifndef FILES_H
#define FILES_H 1

// default includes
#include <stdio.h>
#include <sys/cdefs.h>

// sdl
#include <SDL2/SDL.h>

// my includes
#include "breakout.h"

// Open file called _fileName with mode _mode
FILE *openFile(char *_fileName, char *_mode) __attribute_warn_unused_result__;

// load level from give path
Level *loadLevel(WindowProperties *WindowProperties, char *fileName) __attribute_warn_unused_result__;

// free level
void freeLevels(Array *levels);

#endif