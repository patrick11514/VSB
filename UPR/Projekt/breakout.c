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

void tick(int *frames, SDL_Renderer *renderer, WindowProperties *windowProperties, MainVariables *mainVars)
{
    float scale = windowProperties->scale;

    // clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // for fix text
    SDL_Rect r = {.x = 0, .y = 0, .w = 0, .h = 0};
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &r);

    // FPS
    int FPSHeight = 16;
    if (!renderText(renderer, windowProperties->currentFPS, windowProperties->font, windowProperties->colors->white, 50, FPSHeight, 0, WINDOW_HEIGHT * scale - FPSHeight))
    {
        printf("Error rendering text: %s", SDL_GetError());
    }

    if (windowProperties->currentMenu == MainMenu)
    {
        renderMainMenu(renderer, windowProperties, mainVars);
    }
    else if (windowProperties->currentMenu == Settings)
    {
        renderSettings(renderer, windowProperties, mainVars);
    }

    // rerender
    SDL_RenderPresent(renderer);
    *frames += 1;
}

void renderMainMenu(SDL_Renderer *renderer, WindowProperties *windowProperties, MainVariables *mainVars)
{
    float scale = windowProperties->scale;

    // title text
    TextCoords titleCoords;
    titleCoords.width = 350 * scale;
    titleCoords.height = 100 * scale;
    titleCoords.x = (WINDOW_WIDTH * scale / 2) - (titleCoords.width / 2);
    titleCoords.y = 0;
    renderTitle(renderer, windowProperties, mainVars, &titleCoords);

    // Play text
    int playWidth = 150 * scale;
    int playHeight = 75 * scale;

    int playX = (WINDOW_WIDTH * scale / 2) - (playWidth / 2);
    // title + titleHeight + 50px
    int playY = titleCoords.y + titleCoords.height + 50 * scale;
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
    int settingsWidth = 225 * scale;
    int settingsHeight = 75 * scale;

    int settingsX = (WINDOW_WIDTH * scale / 2) - (settingsWidth / 2);
    // play + playHeight + 50px
    int settingsY = playY + playHeight + 25 * scale;
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

    // highscore text
    int highscoreWidth = 225 * scale;
    int highscoreHeight = 75 * scale;

    int highscoreX = (WINDOW_WIDTH * scale / 2) - (highscoreWidth / 2);
    // settings + settingsHeight + 50px
    int highscoreY = settingsY + settingsHeight + 25 * scale;
    mainVars->mainMenuHighscoreLT.x = highscoreX;
    mainVars->mainMenuHighscoreLT.y = highscoreY;
    mainVars->mainMenuHighscoreRB.x = highscoreX + highscoreWidth;
    mainVars->mainMenuHighscoreRB.y = highscoreY + highscoreHeight;

    SDL_Color highscoreColor = windowProperties->colors->white;
    if (mainVars->mainMenuHighscoreHover)
    {
        highscoreColor = windowProperties->colors->orange;
    }

    if (!renderText(renderer, "HIGHSCORE", windowProperties->font, highscoreColor, highscoreWidth, highscoreHeight, highscoreX, highscoreY))
    {
        printf("Error rendering text: %s", SDL_GetError());
    }

    // exit text
    int exitWidth = 100 * scale;
    int exitHeight = 75 * scale;

    int exitX = (WINDOW_WIDTH * scale / 2) - (exitWidth / 2);
    // highscore + highscoreHeight + 50px
    int exitY = highscoreY + highscoreHeight + 25 * scale;
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

void renderSettings(SDL_Renderer *renderer, WindowProperties *windowProperties, MainVariables *mainVars)
{
    float scale = windowProperties->scale;

    // title text
    TextCoords titleCoords;
    titleCoords.width = 350 * scale;
    titleCoords.height = 100 * scale;
    titleCoords.x = (WINDOW_WIDTH * scale / 2) - (titleCoords.width / 2);
    titleCoords.y = 0;
    renderTitle(renderer, windowProperties, mainVars, &titleCoords);

    // settings text
    int settingsWidth = 225 * scale;
    int settingsHeight = 75 * scale;

    int settingsX = (WINDOW_WIDTH * scale / 2) - (settingsWidth / 2);
    // title + titleHeight + 25px
    int settingsY = titleCoords.y + titleCoords.height + 25;

    if (!renderText(renderer, "SETTINGS", windowProperties->font, windowProperties->colors->white, settingsWidth, settingsHeight, settingsX, settingsY))
    {
        printf("Error rendering text: %s", SDL_GetError());
    }

    // line under settings
    if (!renderRect(renderer, settingsX, settingsY + settingsHeight, settingsWidth, 3, windowProperties->colors->white))
    {
        printf("Error rendering rect: %s", SDL_GetError());
    }

    // scale text
    int scaleWidth = 325 * scale;
    if (scale >= 1.0)
    {
        scaleWidth = 350 * scale;
    }
    int scaleHeight = 50 * scale;

    int scaleTextX = (WINDOW_WIDTH * scale / 2) - (scaleWidth / 2);
    // settings + settingsHeight + 50px
    int scaleTextY = settingsY + settingsHeight + 50 * scale;
    mainVars->settingsScaleLT.x = scaleTextX;
    mainVars->settingsScaleLT.y = scaleTextY;
    mainVars->settingsScaleRB.x = scaleTextX + scaleWidth;
    mainVars->settingsScaleRB.y = scaleTextY + scaleHeight;

    char scaleText[50];

    snprintf(scaleText, sizeof(scaleText), "Current scale: %d%%", (int)(scale * 100));

    SDL_Color scaleColor = windowProperties->colors->white;
    if (mainVars->settingsScaleHover)
    {
        scaleColor = windowProperties->colors->orange;
    }

    if (!renderText(renderer, scaleText, windowProperties->font, scaleColor, scaleWidth, scaleHeight, scaleTextX, scaleTextY))
    {
        printf("Error rendering text: %s", SDL_GetError());
    }

    // back text
    int backWidth = 125 * scale;
    int backHeight = 65 * scale;

    int backX = (WINDOW_WIDTH * scale / 2) - (backWidth / 2);
    // scale + scaleHeight + 50px
    int backY = scaleTextY + scaleHeight + 50 * scale;
    mainVars->settingsBackLT.x = backX;
    mainVars->settingsBackLT.y = backY;
    mainVars->settingsBackRB.x = backX + backWidth;
    mainVars->settingsBackRB.y = backY + backHeight;

    SDL_Color backColor = windowProperties->colors->white;
    if (mainVars->settingsBackHover)
    {
        backColor = windowProperties->colors->orange;
    }

    if (!renderText(renderer, "BACK", windowProperties->font, backColor, backWidth, backHeight, backX, backY))
    {
        printf("Error rendering text: %s", SDL_GetError());
    }
}

void renderTitle(SDL_Renderer *renderer, WindowProperties *windowProperties, MainVariables *mainVars, TextCoords *textCoords)
{
    float scale = windowProperties->scale;

    if (!renderText(renderer, "BREAKOUT", windowProperties->font, windowProperties->colors->yellow, textCoords->width, textCoords->height, textCoords->x, textCoords->y))
    {
        printf("Error rendering text: %s\n", SDL_GetError());
    }

    // paddle
    int paddlePathWidth = 250 * scale;
    // paddle is under title + 10 px up
    mainVars->paddlePosition.y = 0 + textCoords->height - 10;
    mainVars->paddleStartPosition = (WINDOW_WIDTH * scale / 2) - (paddlePathWidth / 2);

    int paddleEnd = mainVars->paddleStartPosition + paddlePathWidth - windowProperties->textures->paddle->width;

    // if paddle position is greater then paddle end + 10 px
    if (mainVars->paddlePosition.x > paddleEnd + 10)
    {
        mainVars->paddlePosition.x = paddleEnd;
    }
    // if paddle position is less than paddle start - 10 px
    if (mainVars->paddlePosition.x < mainVars->paddleStartPosition - 10)
    {
        mainVars->paddlePosition.x = mainVars->paddleStartPosition;
    }

    // move paddle
    if (mainVars->paddlePosition.x > paddleEnd && !mainVars->paddleReverse)
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
}

void checkEvents(SDL_Event *e, bool *quit, WindowProperties *windowProperties, MainVariables *mainVars)
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

            // highscore text
            if (e->motion.x >= mainVars->mainMenuHighscoreLT.x && e->motion.x <= mainVars->mainMenuHighscoreRB.x && e->motion.y >= mainVars->mainMenuHighscoreLT.y && e->motion.y <= mainVars->mainMenuHighscoreRB.y)
            {
                mainVars->mainMenuHighscoreHover = true;
            }
            else
            {
                mainVars->mainMenuHighscoreHover = false;
            }

            // exit text
            if (e->motion.x >= mainVars->mainMenuExitLT.x && e->motion.x <= mainVars->mainMenuExitRB.x && e->motion.y >= mainVars->mainMenuExitLT.y && e->motion.y <= mainVars->mainMenuExitRB.y)
            {
                mainVars->mainMenuExitHover = true;
            }
            else
            {
                mainVars->mainMenuExitHover = false;
            }
        }
        else if (windowProperties->currentMenu == Settings)
        {
            // scale text
            if (e->motion.x >= mainVars->settingsScaleLT.x && e->motion.x <= mainVars->settingsScaleRB.x && e->motion.y >= mainVars->settingsScaleLT.y && e->motion.y <= mainVars->settingsScaleRB.y)
            {
                mainVars->settingsScaleHover = true;
            }
            else
            {
                mainVars->settingsScaleHover = false;
            }

            // back text
            if (e->motion.x >= mainVars->settingsBackLT.x && e->motion.x <= mainVars->settingsBackRB.x && e->motion.y >= mainVars->settingsBackLT.y && e->motion.y <= mainVars->settingsBackRB.y)
            {
                mainVars->settingsBackHover = true;
            }
            else
            {
                mainVars->settingsBackHover = false;
            }
        }
    }

    if (e->type == SDL_MOUSEBUTTONDOWN)
    {
        if (windowProperties->currentMenu == MainMenu)
        {

            // settings
            if (mainVars->mainMenuSettingsHover)
            {
                windowProperties->currentMenu = Settings;
            }

            // quit
            if (mainVars->mainMenuExitHover)
            {
                *quit = true;
            }
        }
        else if (windowProperties->currentMenu == Settings)
        {
            // scale
            if (mainVars->settingsScaleHover)
            {
                float currentScale = windowProperties->scale;

                if (currentScale == 1.0f)
                {
                    currentScale = 1.25f;
                }
                else if (currentScale == 1.25f)
                {
                    currentScale = 1.5f;
                }
                else if (currentScale == 1.5f)
                {
                    currentScale = 0.5f;
                }
                else if (currentScale == 0.5f)
                {
                    currentScale = 0.75f;
                }
                else if (currentScale == 0.75f)
                {
                    currentScale = 1.0f;
                }

                // update window size
                SDL_SetWindowSize(mainVars->window, WINDOW_WIDTH * currentScale, WINDOW_HEIGHT * currentScale);
                windowProperties->scale = currentScale;
            }

            // back
            if (mainVars->settingsBackHover)
            {
                windowProperties->currentMenu = MainMenu;
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