#ifndef BREAKOUT_H
#define BREAKOUT_H 1

#define WINDOW_WIDTH 720
#define WINDOW_HEIGHT 900

#include <SDL2/SDL.h>

void checkEvents(SDL_Event *e, bool *quit);
void tick();

#endif