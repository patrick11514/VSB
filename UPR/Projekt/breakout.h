#ifndef BREAKOUT_H
#define BREAKOUT_H 1

#define WINDOW_WIDTH 720
#define WINDOW_HEIGHT 900

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

typedef enum
{
    MainMenu = 0,
} MenuTypes;

typedef struct Colors
{
    SDL_Color white;
    SDL_Color yellow;
    SDL_Color red;
    SDL_Color green;
    SDL_Color blue;
    SDL_Color black;
} Colors;

typedef struct WindowProperties
{
    float scale;
    TTF_Font *font;
    char currentFPS[255];
    Colors *colors;
    MenuTypes currentMenu;
    Textures *textures;
} WindowProperties;

typedef struct Textures
{
    SDL_Texture *paddle;
} Textures;

// function ran on every SDL event
void checkEvents(SDL_Event *e, bool *quit, float scale);

// function run every frame
void tick(int *frames, SDL_Renderer *renderer, float scale, WindowProperties *windowProperties);

// calculate and display current FPS
void calculateFPS(unsigned long *prevTime, int *frames, char *currentFPS);

// main menu
void renderMainMenu(SDL_Renderer *renderer, float scale, WindowProperties *windowProperties);

#endif