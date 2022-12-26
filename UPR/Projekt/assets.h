#ifndef ASSETS_H
#define ASSETS_H 1

#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef struct Position
{
    float x;
    float y;
} Position;

typedef struct Texture
{
    SDL_Texture *texture;
    int width;
    int height;
} Texture;

typedef struct MainVariables
{
    Position paddlePosition;
    int paddleStartPosition;
    bool paddleReverse;
    int FPS;
} MainVariables;

bool renderText(SDL_Renderer *renderer, char *text, TTF_Font *font, SDL_Color color, int with, int height, int x, int y);

bool renderTexture(SDL_Renderer *renderer, SDL_Texture *texture, int x, int y, int w, int h);

#endif