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
    // main menu paddle position
    Position paddlePosition;
    // main menu paddle start position
    int paddleStartPosition;
    // main menu paddle go reverse if true
    bool paddleReverse;
    // current fps
    int FPS;

    // main menu play button corners LeftTop + RightBottom
    Position mainMenuPlayLT;
    Position mainMenuPlayRB;
    // main menu play button if hover
    bool mainMenuPlayHover;

    // main menu settings button corners LeftTop + RightBottom
    Position mainMenuSettingsLT;
    Position mainMenuSettingsRB;
    // main menu settings button if hover
    bool mainMenuSettingsHover;

    // main menu exit button corners LeftTop + RightBottom
    Position mainMenuExitLT;
    Position mainMenuExitRB;
    // main menu exit button if hover
    bool mainMenuExitHover;

} MainVariables;

bool renderText(SDL_Renderer *renderer, char *text, TTF_Font *font, SDL_Color color, int with, int height, int x, int y);

bool renderTexture(SDL_Renderer *renderer, SDL_Texture *texture, int x, int y, int w, int h);

#endif