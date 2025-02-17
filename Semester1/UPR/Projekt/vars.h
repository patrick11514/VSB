#ifndef VARS_H
#define VARS_H 1

// basic includes
#include <sys/cdefs.h>

// SDL
#include <SDL2/SDL.h>

// my includes
#include "breakout.h"

// load textures
bool loadTextures(WindowProperties *windowProperties, SDL_Renderer *renderer) __attribute_warn_unused_result__;

// free textures
void freeTextures(WindowProperties *windowProperties);

// load window properties
bool loadWindowProperties(WindowProperties *windowProperties, SDL_Renderer *renderer, Colors *colors, float SCALE, SDL_Window *window) __attribute_warn_unused_result__;

// free window properties
void freeWindowProperties(WindowProperties *windowProperties);

// load Main Variables
void loadVars(WindowProperties *windowProperties, MainVariables *vars, SDL_Window *window);

// free Main Variables
void freeVars(MainVariables *vars);

// load colors
void loadColors(Colors *colors);

// init SDL
bool initSDL(SDL_Window **window, SDL_Renderer **renderer, float *scale) __attribute_warn_unused_result__;

// load default levels
void loadDefaultLevels(WindowProperties *windowProperties);

#endif