// basic includes
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

// my includes
#include "highscores.h"
#include "breakout.h"
#include "assets.h"

// SDL2
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

//======================= [ BREAKOUT FUNCTIONS ] ==========================

void tick(int *frames, SDL_Renderer *renderer, float scale, WindowProperties *windowProperties, MainVariables *mainVars)
{

    // clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // for fix text
    SDL_Rect r = {.x = 0, .y = 0, .w = 0, .h = 0};
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &r);

    // FPS
    int FPSHeight = 16;
    if (!renderText(renderer, windowProperties->currentFPS, windowProperties->font, windowProperties->colors->white, 50, FPSHeight, 0, WINDOW_HEIGHT * windowProperties->scale - FPSHeight))
    {
        printf("Error rendering text: %s", SDL_GetError());
    }

    if (windowProperties->currentMenu == MainMenu)
    {
        renderMainMenu(renderer, scale, windowProperties, mainVars);
    }

    // rerender
    SDL_RenderPresent(renderer);
    *frames += 1;
}

void renderMainMenu(SDL_Renderer *renderer, float scale, WindowProperties *windowProperties, MainVariables *mainVars)
{
    // title
    int titleWidth = 275;
    int titleHeight = 80;

    if (!renderText(renderer, "BREAKOUT", windowProperties->font, windowProperties->colors->yellow, titleWidth, titleHeight, (WINDOW_WIDTH * windowProperties->scale / 2) - (titleWidth / 2), 0))
    {
        printf("Error rendering text: %s\n", SDL_GetError());
    }

    // paddle
    int paddlePathWidth = 250;
    mainVars->paddlePosition.y = titleHeight;

    mainVars->paddleStartPosition = (WINDOW_WIDTH * windowProperties->scale / 2) - (paddlePathWidth / 2);

    if (mainVars->paddlePosition.x > mainVars->paddleStartPosition + paddlePathWidth - windowProperties->textures->paddle->width && !mainVars->paddleReverse)
    {
        mainVars->paddleReverse = true;
    }
    else if (mainVars->paddlePosition.x < mainVars->paddleStartPosition && mainVars->paddleReverse)
    {
        mainVars->paddleReverse = false;
    }
    else
    {
        if (mainVars->FPS > 0)
        {
            if (mainVars->paddleReverse)
            {
                mainVars->paddlePosition.x -= (MOVE_FPS / mainVars->FPS);
            }
            else
            {
                mainVars->paddlePosition.x += (MOVE_FPS / mainVars->FPS);
            }
        }
    }

    printf("%f\n", mainVars->paddlePosition.x);

    Texture *paddle = windowProperties->textures->paddle;
    if (!renderTexture(renderer, paddle->texture, mainVars->paddlePosition.x, mainVars->paddlePosition.y, paddle->width, paddle->height))
    {
        printf("Error rendering texture: %s\n", SDL_GetError());
    }
}

void checkEvents(SDL_Event *e, bool *quit, float scale)
{
}

void calculateFPS(unsigned long *prevTime, int *frames, char *currentFPS, MainVariables *vars)
{
    // calculate FPS
    unsigned long currentTime = time(NULL);
    if (currentTime - (*prevTime) >= 1)
    {
        snprintf(currentFPS, 10, "FPS: %d", *frames);
        vars->FPS = *frames;
        *frames = 0;
        *prevTime = currentTime;
    }
}

//========================================================================