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
    SDL_Window *window;

    // paddle under title position
    Position paddlePosition;
    // paddle under title start position
    int paddleStartPosition;
    // paddle under title go reverse if true
    bool paddleReverse;

    // current fps
    int FPS;

    // MAIN MENU BUTTONS
    //  main menu play button corners LeftTop + RightBottom
    Position mainMenuPlayLT;
    Position mainMenuPlayRB;
    // main menu play button if hover
    bool mainMenuPlayHover;

    // main menu settings button corners LeftTop + RightBottom
    Position mainMenuSettingsLT;
    Position mainMenuSettingsRB;
    // main menu settings button if hover
    bool mainMenuSettingsHover;

    // main menu highscore button corners LeftTop + RightBottom
    Position mainMenuHighscoreLT;
    Position mainMenuHighscoreRB;
    // main menu highscore button if hover
    bool mainMenuHighscoreHover;

    // main menu exit button corners LeftTop + RightBottom
    Position mainMenuExitLT;
    Position mainMenuExitRB;
    // main menu exit button if hover
    bool mainMenuExitHover;

    // SETTING BUTTONS
    // scale button corners LeftTop + RightBottom
    Position settingsScaleLT;
    Position settingsScaleRB;
    // scale button if hover
    bool settingsScaleHover;

    // back button corners LeftTop + RightBottom
    Position settingsBackLT;
    Position settingsBackRB;
    // back button if hover
    bool settingsBackHover;

} MainVariables;

bool renderText(SDL_Renderer *renderer, char *text, TTF_Font *font, SDL_Color color, int with, int height, int x, int y);

bool renderTexture(SDL_Renderer *renderer, SDL_Texture *texture, int x, int y, int w, int h);

bool renderRect(SDL_Renderer *renderer, int x, int y, int w, int h, SDL_Color color);

#endif