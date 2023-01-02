// basic includes
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

// SDL2
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

// my includes
#include "highscores.h"
#include "breakout.h"
#include "assets.h"
#include "vars.h"
#include "args.h"

int main(int argc, char **argv)
{

    // init sdl
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    float SCALE;
    if (!initSDL(&window, &renderer, &SCALE))
    {
        return 1;
    }

    // =========================================== [ VARIABLES ] ===========================================

    // some variables
    SDL_Event e;
    bool quit = false;
    int frames = 0;
    unsigned long prevTime = time(NULL);

    // =========================================== [ COLORS ] ===========================================

    // colors
    Colors *colors = (Colors *)malloc(sizeof(Colors));
    loadColors(colors);

    // =====================================  [ WIDOW PROPERTIES] =====================================

    // allocate memory for Window Properties
    WindowProperties *windowProperties = (WindowProperties *)malloc(sizeof(WindowProperties));
    if (!loadWindowProperties(windowProperties, renderer, colors, SCALE, window))
    {
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    // =========================================== [ TEXTURES ] ===========================================

    // load textures
    if (!loadTextures(windowProperties, renderer))
    {
        fprintf(stderr, "Unable to load textures.");
        freeTextures(windowProperties);
        freeWindowProperties(windowProperties);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // ========================================= [ DEFAULT LEVELS ] ===========================================

    // levels
    windowProperties->levels = arrayInit(ARRAY_DEFAULT_CAPACITY);
    if (!windowProperties->levels)
    {
        fprintf(stderr, "Unable to allocate memory for levels Array.\n");
        exit(1);
    }

    // =========================================== [ ARGUMENTS ] ===========================================
    bool levels = false;
    bool includeDefaultLevels = false;

    if (loadArgs(argc, argv, &levels, &includeDefaultLevels, windowProperties))
    {
        freeTextures(windowProperties);
        freeWindowProperties(windowProperties);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // =====================================================================================

    if (!levels || (levels && includeDefaultLevels))
    {
        loadDefaultLevels(windowProperties);
    }

    // ========================================= [ MAIN VARIABLES ] ===========================================

    MainVariables *vars = (MainVariables *)malloc(sizeof(MainVariables));

    loadVars(windowProperties, vars, window);

    // game loop
    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }

            checkEvents(&e, &quit, windowProperties, vars);
        }

        calculateFPS(&prevTime, &frames, windowProperties->currentFPS, vars);
        tick(&frames, renderer, windowProperties, vars);
    }

    freeLevels(windowProperties->levels);

    freeTextures(windowProperties);

    freeWindowProperties(windowProperties);

    freeVars(vars);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
