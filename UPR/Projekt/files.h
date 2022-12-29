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
Level *loadLevel(SDL_Renderer *renderer, WindowProperties *WindowProperties, char *fileName);

#endif