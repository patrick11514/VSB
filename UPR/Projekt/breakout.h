#ifndef BREAKOUT_H
#define BREAKOUT_H 1

#define WINDOW_WIDTH 720
#define WINDOW_HEIGHT 900

#define MOVE_FPS 60.0f

#define HIGHSCORES_PER_PAGE 8

// default includes
#include <stdbool.h>

// sdl
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

// my includes
#include "assets.h"

typedef enum
{
    MainMenu = 0,
    Game = 1,
    Settings = 2,
    Highscore = 3,
} MenuTypes;

typedef struct Colors
{
    SDL_Color white;
    SDL_Color yellow;
    SDL_Color red;
    SDL_Color green;
    SDL_Color blue;
    SDL_Color black;
    SDL_Color orange;
} Colors;

typedef struct Textures
{
    Texture *paddle;
    Texture *buttonUp;
    Texture *buttonUpHover;
    Texture *buttonDown;
    Texture *buttonDownHover;
} Textures;

typedef struct WindowProperties
{
    // scale of window
    float scale;
    // Roboto Font
    TTF_Font *font;
    // current FPS text
    char currentFPS[10];
    // colors
    Colors *colors;
    // current menu
    MenuTypes currentMenu;
    // loaded textures
    Textures *textures;
} WindowProperties;

typedef struct TextCoords
{
    int x;
    int y;
    int width;
    int height;
} TextCoords;

// function ran on every SDL event
void checkEvents(SDL_Event *e, bool *quit, WindowProperties *windowProperties, MainVariables *mainVars);

// function run every frame
void tick(int *frames, SDL_Renderer *renderer, WindowProperties *windowProperties, MainVariables *vars);

// calculate and display current FPS
void calculateFPS(unsigned long *prevTime, int *frames, char *currentFPS, MainVariables *vars);

// main menu
void renderMainMenu(SDL_Renderer *renderer, WindowProperties *windowProperties, MainVariables *vars);

// settings
void renderSettings(SDL_Renderer *renderer, WindowProperties *windowProperties, MainVariables *mainVars);

// highscore screen
void renderHighscore(SDL_Renderer *renderer, WindowProperties *windowProperties, MainVariables *mainVars);

// render title function
void renderTitle(SDL_Renderer *renderer, WindowProperties *windowProperties, MainVariables *mainVars, TextCoords *textCoords);

#endif