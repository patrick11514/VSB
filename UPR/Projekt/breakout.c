// basic includes
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

// my includes
#include "highscores.h"
#include "breakout.h"

// SDL2
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

//======================= [ BREAKOUT FUNCTIONS ] ==========================

void tick(int *frames, SDL_Renderer *renderer, float scale, WindowProperties *windowProperties)
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
    SDL_Rect FPSRect = {.x = 0, .y = WINDOW_HEIGHT * windowProperties->scale - FPSHeight, .w = 50, .h = FPSHeight};

    SDL_Surface *FPSSurface = TTF_RenderText_Solid(windowProperties->font, windowProperties->currentFPS, windowProperties->colors->white);
    SDL_Texture *FPSTexture = SDL_CreateTextureFromSurface(renderer, FPSSurface);
    SDL_RenderCopy(renderer, FPSTexture, NULL, &FPSRect);

    if (windowProperties->currentMenu == MainMenu)
    {
        renderMainMenu(renderer, scale, windowProperties);
    }

    // rerender
    SDL_RenderPresent(renderer);

    // free memory
    SDL_DestroyTexture(FPSTexture);
    SDL_FreeSurface(FPSSurface);
    *frames += 1;
}

void renderMainMenu(SDL_Renderer *renderer, float scale, WindowProperties *windowProperties)
{
    // title
    int titleWidth = 275;

    SDL_Rect TitleRect = {.x = (WINDOW_WIDTH * windowProperties->scale / 2) - (titleWidth / 2), .y = 0, .w = titleWidth, .h = 80};
    SDL_Surface *TitleSurface = TTF_RenderText_Solid(windowProperties->font, "BREAKOUT", windowProperties->colors->yellow);
    SDL_Texture *TitleTexture = SDL_CreateTextureFromSurface(renderer, TitleSurface);
    SDL_RenderCopy(renderer, TitleTexture, NULL, &TitleRect);

    int paddlePos;
    if (!paddlePos)
    {
        paddlePos = 0;
    }
    else
    {
        if (paddlePos > 200)
        {
            paddlePos = 0;
        }
        else
        {
            paddlePos++;
        }
    }

        // free memory
    SDL_DestroyTexture(TitleTexture);
    SDL_FreeSurface(TitleSurface);
}

void checkEvents(SDL_Event *e, bool *quit, float scale)
{
}

void calculateFPS(unsigned long *prevTime, int *frames, char *currentFPS)
{
    // calculate FPS
    unsigned long currentTime = time(NULL);
    if (currentTime - (*prevTime) >= 1)
    {
        snprintf(currentFPS, 10, "FPS: %d", *frames);
        *frames = 0;
        *prevTime = currentTime;
    }
}

//========================================================================