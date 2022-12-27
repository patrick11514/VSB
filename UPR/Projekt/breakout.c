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
    int titleWidth = 350;
    int titleHeight = 100;

    int titleX = (WINDOW_WIDTH * windowProperties->scale / 2) - (titleWidth / 2);
    int titleY = 0;

    if (!renderText(renderer, "BREAKOUT", windowProperties->font, windowProperties->colors->yellow, titleWidth, titleHeight, titleX, titleY))
    {
        printf("Error rendering text: %s\n", SDL_GetError());
    }

    // paddle
    int paddlePathWidth = 250;
    // paddle is under title + 10 px up
    mainVars->paddlePosition.y = 0 + titleHeight - 10;
    mainVars->paddleStartPosition = (WINDOW_WIDTH * windowProperties->scale / 2) - (paddlePathWidth / 2);

    // move paddle
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

    // render paddle
    Texture *paddle = windowProperties->textures->paddle;
    if (!renderTexture(renderer, paddle->texture, mainVars->paddlePosition.x, mainVars->paddlePosition.y, paddle->width, paddle->height))
    {
        printf("Error rendering texture: %s\n", SDL_GetError());
    }

    // Play text
    int playWidth = 150;
    int playHeight = 75;

    // start + title + paddle + 50 px
    int playX = (WINDOW_WIDTH * windowProperties->scale / 2) - (playWidth / 2);
    int playY = titleY + titleHeight + 50;
    mainVars->mainMenuPlayLT.x = playX;
    mainVars->mainMenuPlayLT.y = playY;
    mainVars->mainMenuPlayRB.x = playX + playWidth;
    mainVars->mainMenuPlayRB.y = playY + playHeight;

    SDL_Color playColor = windowProperties->colors->white;
    if (mainVars->mainMenuPlayHover)
    {
        playColor = windowProperties->colors->orange;
    }

    if (!renderText(renderer, "PLAY", windowProperties->font, playColor, playWidth, playHeight, playX, playY))
    {
        printf("Error rendering text: %s", SDL_GetError());
    }

    // settings text
    int settingsWidth = 225;
    int settingsHeight = 75;

    // start + title + paddle + 50 px + play + 50 px
    int settingsX = (WINDOW_WIDTH * windowProperties->scale / 2) - (settingsWidth / 2);
    int settingsY = playY + playHeight + 25;
    mainVars->mainMenuSettingsLT.x = settingsX;
    mainVars->mainMenuSettingsLT.y = settingsY;
    mainVars->mainMenuSettingsRB.x = settingsX + settingsWidth;
    mainVars->mainMenuSettingsRB.y = settingsY + settingsHeight;

    SDL_Color settingsColor = windowProperties->colors->white;
    if (mainVars->mainMenuSettingsHover)
    {
        settingsColor = windowProperties->colors->orange;
    }

    if (!renderText(renderer, "SETTINGS", windowProperties->font, settingsColor, settingsWidth, settingsHeight, settingsX, settingsY))
    {
        printf("Error rendering text: %s", SDL_GetError());
    }

    // exit
    int exitWidth = 100;
    int exitHeight = 75;

    // start + title + paddle + 50 px + play + 50 px + settings + 50 px
    int exitX = (WINDOW_WIDTH * windowProperties->scale / 2) - (exitWidth / 2);
    int exitY = settingsY + settingsHeight + 25;
    mainVars->mainMenuExitLT.x = exitX;
    mainVars->mainMenuExitLT.y = exitY;
    mainVars->mainMenuExitRB.x = exitX + exitWidth;
    mainVars->mainMenuExitRB.y = exitY + exitHeight;

    SDL_Color exitColor = windowProperties->colors->white;
    if (mainVars->mainMenuExitHover)
    {
        exitColor = windowProperties->colors->orange;
    }

    if (!renderText(renderer, "EXIT", windowProperties->font, exitColor, exitWidth, exitHeight, exitX, exitY))
    {
        printf("Error rendering text: %s", SDL_GetError());
    }
}

void checkEvents(SDL_Event *e, bool *quit, float scale, WindowProperties *windowProperties, MainVariables *mainVars)
{
    if (e->type == SDL_MOUSEMOTION)
    {
        if (windowProperties->currentMenu == MainMenu)
        {
            // play text
            if (e->motion.x >= mainVars->mainMenuPlayLT.x && e->motion.x <= mainVars->mainMenuPlayRB.x && e->motion.y >= mainVars->mainMenuPlayLT.y && e->motion.y <= mainVars->mainMenuPlayRB.y)
            {
                mainVars->mainMenuPlayHover = true;
            }
            else
            {
                mainVars->mainMenuPlayHover = false;
            }

            // settings text
            if (e->motion.x >= mainVars->mainMenuSettingsLT.x && e->motion.x <= mainVars->mainMenuSettingsRB.x && e->motion.y >= mainVars->mainMenuSettingsLT.y && e->motion.y <= mainVars->mainMenuSettingsRB.y)
            {
                mainVars->mainMenuSettingsHover = true;
            }
            else
            {
                mainVars->mainMenuSettingsHover = false;
            }

            // exit
            if (e->motion.x >= mainVars->mainMenuExitLT.x && e->motion.x <= mainVars->mainMenuExitRB.x && e->motion.y >= mainVars->mainMenuExitLT.y && e->motion.y <= mainVars->mainMenuExitRB.y)
            {
                mainVars->mainMenuExitHover = true;
            }
            else
            {
                mainVars->mainMenuExitHover = false;
            }
        }
    }
    if (e->type == SDL_MOUSEBUTTONDOWN)
    {
        if (windowProperties->currentMenu == MainMenu)
        {

            // quit
            if (mainVars->mainMenuExitHover)
            {
                *quit = true;
            }
        }
    }
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