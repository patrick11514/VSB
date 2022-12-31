// basic includes
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
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

//======================= [ BREAKOUT FUNCTIONS ] ==========================

void tick(int *frames, SDL_Renderer *renderer, WindowProperties *windowProperties, MainVariables *mainVars)
{
    float scale = windowProperties->scale;

    // clear screen
    SDL_SetRenderDrawColor(renderer, 26, 25, 24, 255);
    SDL_RenderClear(renderer);

    // for fix text
    SDL_Rect r = {.x = 0, .y = 0, .w = 0, .h = 0};
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &r);

    // FPS
    int FPSHeight = 16;
    if (!renderText(renderer, windowProperties->currentFPS, windowProperties->font, windowProperties->colors->white, 50, FPSHeight, 0, WINDOW_HEIGHT * scale - FPSHeight))
    {
        fprintf(stderr, "Error rendering text: %s", SDL_GetError());
    }

    if (windowProperties->currentMenu == MainMenu)
    {
        renderMainMenu(renderer, windowProperties, mainVars);
    }
    else if (windowProperties->currentMenu == Settings)
    {
        renderSettings(renderer, windowProperties, mainVars);
    }
    else if (windowProperties->currentMenu == Highscore)
    {
        renderHighscore(renderer, windowProperties, mainVars);
    }
    else if (windowProperties->currentMenu == LevelSelect)
    {
        renderLevelSelect(renderer, windowProperties, mainVars);
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
        fprintf(stderr, "Error rendering text: %s", SDL_GetError());
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
        fprintf(stderr, "Error rendering text: %s", SDL_GetError());
    }

    // highscores text
    int highscoresWidth = 300 * scale;
    int highscoresHeight = 75 * scale;

    int highscoresX = (WINDOW_WIDTH * scale / 2) - (highscoresWidth / 2);
    // settings + settingsHeight + 50px
    int highscoresY = settingsY + settingsHeight + 25 * scale;
    mainVars->mainMenuHighscoresLT.x = highscoresX;
    mainVars->mainMenuHighscoresLT.y = highscoresY;
    mainVars->mainMenuHighscoresRB.x = highscoresX + highscoresWidth;
    mainVars->mainMenuHighscoresRB.y = highscoresY + highscoresHeight;

    SDL_Color highscoresColor = windowProperties->colors->white;
    if (mainVars->mainMenuHighscoresHover)
    {
        highscoresColor = windowProperties->colors->orange;
    }

    if (!renderText(renderer, "HIGHSCORES", windowProperties->font, highscoresColor, highscoresWidth, highscoresHeight, highscoresX, highscoresY))
    {
        fprintf(stderr, "Error rendering text: %s", SDL_GetError());
    }

    // exit text
    int exitWidth = 100 * scale;
    int exitHeight = 75 * scale;

    int exitX = (WINDOW_WIDTH * scale / 2) - (exitWidth / 2);
    // highscores + highscoreHeight + 50px
    int exitY = highscoresY + highscoresHeight + 25 * scale;
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
        fprintf(stderr, "Error rendering text: %s", SDL_GetError());
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
        fprintf(stderr, "Error rendering text: %s", SDL_GetError());
    }

    // line under settings
    if (!renderRect(renderer, settingsX, settingsY + settingsHeight, settingsWidth, 3, windowProperties->colors->white))
    {
        fprintf(stderr, "Error rendering rect: %s", SDL_GetError());
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
        fprintf(stderr, "Error rendering text: %s", SDL_GetError());
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
        fprintf(stderr, "Error rendering text: %s", SDL_GetError());
    }
}

void renderHighscore(SDL_Renderer *renderer, WindowProperties *windowProperties, MainVariables *mainVars)
{
    float scale = windowProperties->scale;

    // title text
    TextCoords titleCoords;
    titleCoords.width = 350 * scale;
    titleCoords.height = 100 * scale;
    titleCoords.x = (WINDOW_WIDTH * scale / 2) - (titleCoords.width / 2);
    titleCoords.y = 0;
    renderTitle(renderer, windowProperties, mainVars, &titleCoords);

    // highscores text
    int highscoreWidth = 300 * scale;
    int highscoreHeight = 75 * scale;

    int highscoreX = (WINDOW_WIDTH * scale / 2) - (highscoreWidth / 2);
    // title + titleHeight + 25px
    int highscoreY = titleCoords.y + titleCoords.height + 25;

    if (!renderText(renderer, "HIGHSCORES", windowProperties->font, windowProperties->colors->white, highscoreWidth, highscoreHeight, highscoreX, highscoreY))
    {
        fprintf(stderr, "Error rendering text: %s", SDL_GetError());
    }

    // line under highscores
    if (!renderRect(renderer, highscoreX, highscoreY + highscoreHeight, highscoreWidth, 3, windowProperties->colors->white))
    {
        fprintf(stderr, "Error rendering rect: %s", SDL_GetError());
    }

    Highscores *highscores = windowProperties->highscores;

    // Y coord for back button edited in if statement
    int backY;
    int backHeight = 65 * scale;

    if (!highscores)
    {
        // read file
        FILE *file = openFile("highscores.txt", "rt");

        // if file doesn't exist, create new highscores
        if (!file)
        {
            highscores = (Highscores *)malloc(sizeof(Highscores));
            highscores->count = 0;
            highscores->players = arrayInit(ARRAY_DEFAULT_CAPACITY);
            highscores->scores = arrayInit(ARRAY_DEFAULT_CAPACITY);
        }
        else
        {
            // read highscores from file
            highscores = readHighscores(file);
            fclose(file);
            sortHighscores(highscores);
        }

        // save loaded highscores
        windowProperties->highscores = highscores;

        // init variables for highscore rendering
        mainVars->highscoresOffset = 0;
        mainVars->highscoresUpButton = false;
        if (highscores->count > HIGHSCORES_PER_PAGE)
        {
            mainVars->highscoresDownButton = true;
        }
        else
        {
            mainVars->highscoresDownButton = false;
        }
    }

    if (highscores->count == 0)
    {
        // no highscores
        int noHighscoresWidth = 300 * scale;
        int noHighscoresHeight = 50 * scale;

        int noHighscoresX = (WINDOW_WIDTH * scale / 2) - (noHighscoresWidth / 2);
        // highscore + highscoreHeight + 50px
        int noHighscoresY = highscoreY + highscoreHeight + 50 * scale;

        if (!renderText(renderer, "No highscores yet", windowProperties->font, windowProperties->colors->red, noHighscoresWidth, noHighscoresHeight, noHighscoresX, noHighscoresY))
        {
            fprintf(stderr, "Error rendering text: %s", SDL_GetError());
        }

        // back button
        // 40 pixels from down
        backY = WINDOW_HEIGHT * scale - 40 * scale - backHeight;
    }
    else
    {
        // textures
        Texture *buttonUp;
        Texture *buttonDown;

        if (mainVars->highscoresPrevHover)
        {
            buttonUp = windowProperties->textures->buttonUpHover;
        }
        else
        {
            buttonUp = windowProperties->textures->buttonUp;
        }

        if (mainVars->highscoresNextHover)
        {
            buttonDown = windowProperties->textures->buttonDownHover;
        }
        else
        {
            buttonDown = windowProperties->textures->buttonDown;
        }

        int buttonUpX = (WINDOW_WIDTH * scale / 2) - (buttonUp->width * scale / 2);
        // highscore + highscoreHeight + 20px (spacing)
        int buttonUpY = highscoreY + highscoreHeight + 20;

        // if enabled render up button
        if (mainVars->highscoresUpButton)
        {

            mainVars->highscoresPrevLT.x = buttonUpX;
            mainVars->highscoresPrevLT.y = buttonUpY;
            mainVars->highscoresPrevRB.x = buttonUpX + buttonUp->width * scale;
            mainVars->highscoresPrevRB.y = buttonUpY + buttonUp->height * scale;

            if (!renderTexture(renderer, buttonUp->texture, buttonUpX, buttonUpY, buttonUp->width * scale, buttonUp->height * scale))
            {
                fprintf(stderr, "Error rendering texture: %s", SDL_GetError());
            }
        }

        // highscores
        // buttonUp + buttonUpHeight + 10px
        int highscoreY = buttonUpY + buttonUp->height * scale + 10 * scale;

        int end = mainVars->highscoresOffset * HIGHSCORES_PER_PAGE + HIGHSCORES_PER_PAGE;

        if (end > highscores->count)
        {
            end = highscores->count;
        }

        for (int i = mainVars->highscoresOffset * HIGHSCORES_PER_PAGE; i < end; i++)
        {
            char text[255];

            snprintf(text, 255, "%s - %s", (char *)highscores->players->data[i], (char *)highscores->scores->data[i]);

            int len = count_utf8_code_points(text);

            // 20px per char
            int x = (WINDOW_WIDTH * scale / 2) - (len * 20 * scale / 2);

            if (!renderText(renderer, text, windowProperties->font, windowProperties->colors->white, len * 20 * scale, 50 * scale, x, highscoreY))
            {
                fprintf(stderr, "Error rendering text: %s", SDL_GetError());
            }

            // 50px per highscore
            highscoreY += 50 * scale;
        }

        int buttonDownX = (WINDOW_WIDTH * scale / 2) - (buttonDown->width * scale / 2);
        // highscoreY - 50px (because of last highscore add 50px) + 10px (spacing) + buttonDownHeight
        int buttonDownY = highscoreY - 50 + 10 + buttonDown->height;

        // if enabled render down button
        if (mainVars->highscoresDownButton)
        {

            mainVars->highscoresNextLT.x = buttonDownX;
            mainVars->highscoresNextLT.y = buttonDownY;
            mainVars->highscoresNextRB.x = buttonDownX + buttonDown->width * scale;
            mainVars->highscoresNextRB.y = buttonDownY + buttonDown->height * scale;

            if (!renderTexture(renderer, buttonDown->texture, buttonDownX, buttonDownY, buttonDown->width * scale, buttonDown->height * scale))
            {
                fprintf(stderr, "Error rendering texture: %s", SDL_GetError());
            }
        }

        // back button
        //  buttonDown + buttonDownHeight + 20px (spacing)
        backY = buttonDownY + buttonDown->height * scale + 20 * scale;
    }

    int backWidth = 125 * scale;
    int backX = (WINDOW_WIDTH * scale / 2) - (backWidth / 2);

    // back text
    mainVars->highscoresBackLT.x = backX;
    mainVars->highscoresBackLT.y = backY;
    mainVars->highscoresBackRB.x = backX + backWidth;
    mainVars->highscoresBackRB.y = backY + backHeight;

    SDL_Color backColor = windowProperties->colors->white;
    if (mainVars->highscoresBackHover)
    {
        backColor = windowProperties->colors->orange;
    }

    if (!renderText(renderer, "BACK", windowProperties->font, backColor, backWidth, backHeight, backX, backY))
    {
        fprintf(stderr, "Error rendering text: %s", SDL_GetError());
    }
}

void renderLevelSelect(SDL_Renderer *renderer, WindowProperties *windowProperties, MainVariables *mainVars)
{
    float scale = windowProperties->scale;

    // title text
    TextCoords titleCoords;
    titleCoords.width = 350 * scale;
    titleCoords.height = 100 * scale;
    titleCoords.x = (WINDOW_WIDTH * scale / 2) - (titleCoords.width / 2);
    titleCoords.y = 0;
    renderTitle(renderer, windowProperties, mainVars, &titleCoords);

    // level select text
    int levelWidth = 350 * scale;
    int levelHeight = 85 * scale;

    int levelX = (WINDOW_WIDTH * scale / 2) - (levelWidth / 2);
    int levelY = titleCoords.y + titleCoords.height + 10 * scale;

    if (!renderText(renderer, "LEVEL SELECT", windowProperties->font, windowProperties->colors->white, levelWidth, levelHeight, levelX, levelY))
    {
        fprintf(stderr, "Error rendering text: %s", SDL_GetError());
    }

    // line under level select
    if (!renderRect(renderer, levelX, levelY + levelHeight, levelWidth, 3, windowProperties->colors->white))
    {
        fprintf(stderr, "Error rendering rect: %s", SDL_GetError());
    }

    // levels
    Array *levels = windowProperties->levels;

    if (mainVars->levelSelectOffset > 0)
    {
        mainVars->levelSelectUpButton = true;
    }
    else
    {
        mainVars->levelSelectUpButton = false;
    }

    if (levels->size > mainVars->levelSelectOffset * LEVELS_PER_PAGE + LEVELS_PER_PAGE)
    {
        mainVars->levelSelectDownButton = true;
    }
    else
    {
        mainVars->levelSelectDownButton = false;
    }

    // buttons
    int upButtonY = levelY + levelHeight + 10 * scale;

    if (mainVars->levelSelectUpButton)
    {
        SDL_Texture *texture = windowProperties->textures->buttonUp->texture;
        if (mainVars->levelSelectBackHover)
        {
            texture = windowProperties->textures->buttonUpHover->texture;
        }

        int upButtonX = (WINDOW_WIDTH * scale / 2) - (windowProperties->textures->buttonUp->width * scale / 2);

        mainVars->levelSelectPrevLT.x = upButtonX;
        mainVars->levelSelectPrevLT.y = upButtonY;
        mainVars->levelSelectPrevRB.x = upButtonX + windowProperties->textures->buttonUp->width * scale;
        mainVars->levelSelectPrevRB.y = upButtonY + windowProperties->textures->buttonUp->height * scale;

        if (!renderTexture(renderer, texture, upButtonX, upButtonY, windowProperties->textures->buttonUp->width * scale, windowProperties->textures->buttonUp->height * scale))
        {
            fprintf(stderr, "Error rendering texture: %s", SDL_GetError());
        }
    }

    // render levels
    int end = mainVars->levelSelectOffset * LEVELS_PER_PAGE + LEVELS_PER_PAGE;

    if (end > levels->size)
    {
        end = levels->size;
    }

    int levelTextY = upButtonY + windowProperties->textures->buttonUp->height * scale + 10 * scale;
    for (int i = mainVars->levelSelectOffset * LEVELS_PER_PAGE; i < end; i++)
    {
        Level *level = levels->data[i];

        char *text = level->description;
        int len = count_utf8_code_points(text);

        printf("'%s' - %d\n", text, len);

        // 20px per char
        int levelTextX = (WINDOW_WIDTH * scale / 2) - (len * 20 * scale / 2);

        SDL_Color color = windowProperties->colors->white;

        if (!renderText(renderer, level->description, windowProperties->font, color, len * 20 * scale, 50 * scale, levelTextX, levelTextY))
        {
            fprintf(stderr, "Error rendering text: %s", SDL_GetError());
        }

        // 50px per level
        levelTextY += 50 * scale;
    }

    int downButtonY = levelTextY - +10 * scale;

    if (mainVars->levelSelectDownButton)
    {
        int downButtonX = (WINDOW_WIDTH * scale / 2) - (windowProperties->textures->buttonDown->width * scale / 2);

        SDL_Texture *texture = windowProperties->textures->buttonDown->texture;
        if (mainVars->levelSelectNextHover)
        {
            texture = windowProperties->textures->buttonDownHover->texture;
        }

        mainVars->levelSelectBackLT.x = downButtonX;
        mainVars->levelSelectBackLT.y = downButtonY;
        mainVars->levelSelectBackRB.x = downButtonX + windowProperties->textures->buttonDown->width * scale;
        mainVars->levelSelectBackRB.y = downButtonY + windowProperties->textures->buttonDown->height * scale;

        if (!renderTexture(renderer, texture, downButtonX, downButtonY, windowProperties->textures->buttonDown->width * scale, windowProperties->textures->buttonDown->height * scale))
        {
            fprintf(stderr, "Error rendering texture: %s", SDL_GetError());
        }
    }

    // back text
    int backWidth = 125 * scale;
    int backHeight = 65 * scale;

    int backX = (WINDOW_WIDTH * scale / 2) - (backWidth / 2);
    // scale + scaleHeight + 50px
    int backY = downButtonY + windowProperties->textures->buttonDown->height * scale + 50 * scale;
    mainVars->levelSelectBackLT.x = backX;
    mainVars->levelSelectBackLT.y = backY;
    mainVars->levelSelectBackRB.x = backX + backWidth;
    mainVars->levelSelectBackRB.y = backY + backHeight;

    SDL_Color backColor = windowProperties->colors->white;
    if (mainVars->levelSelectBackHover)
    {
        backColor = windowProperties->colors->orange;
    }

    if (!renderText(renderer, "BACK", windowProperties->font, backColor, backWidth, backHeight, backX, backY))
    {
        fprintf(stderr, "Error rendering text: %s", SDL_GetError());
    }
}

void renderTitle(SDL_Renderer *renderer, WindowProperties *windowProperties, MainVariables *mainVars, TextCoords *textCoords)
{
    float scale = windowProperties->scale;

    if (!renderText(renderer, "BREAKOUT", windowProperties->font, windowProperties->colors->yellow, textCoords->width, textCoords->height, textCoords->x, textCoords->y))
    {
        fprintf(stderr, "Error rendering text: %s\n", SDL_GetError());
    }

    // paddle
    int paddlePathWidth = 250 * scale;
    // paddle is under title + 10 px up
    mainVars->paddlePosition.y = 0 + textCoords->height - 10;
    mainVars->paddleStartPosition = (WINDOW_WIDTH * scale / 2) - (paddlePathWidth / 2);

    int paddleEnd = mainVars->paddleStartPosition + paddlePathWidth - windowProperties->textures->paddle->width * scale;

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
                mainVars->paddlePosition.x -= (MOVE_FPS / mainVars->FPS) * scale;
            }
            else
            {
                mainVars->paddlePosition.x += (MOVE_FPS / mainVars->FPS) * scale;
            }
        }
    }

    // render paddle
    Texture *paddle = windowProperties->textures->paddle;
    if (!renderTexture(renderer, paddle->texture, mainVars->paddlePosition.x, mainVars->paddlePosition.y, paddle->width * scale, paddle->height * scale))
    {
        fprintf(stderr, "Error rendering texture: %s\n", SDL_GetError());
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

            // highscores text
            if (e->motion.x >= mainVars->mainMenuHighscoresLT.x && e->motion.x <= mainVars->mainMenuHighscoresRB.x && e->motion.y >= mainVars->mainMenuHighscoresLT.y && e->motion.y <= mainVars->mainMenuHighscoresRB.y)
            {
                mainVars->mainMenuHighscoresHover = true;
            }
            else
            {
                mainVars->mainMenuHighscoresHover = false;
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
        else if (windowProperties->currentMenu == Highscore)
        {

            // prev button
            if (e->motion.x >= mainVars->highscoresPrevLT.x && e->motion.x <= mainVars->highscoresPrevRB.x && e->motion.y >= mainVars->highscoresPrevLT.y && e->motion.y <= mainVars->highscoresPrevRB.y)
            {
                mainVars->highscoresPrevHover = true;
            }
            else
            {
                mainVars->highscoresPrevHover = false;
            }

            // next button
            if (e->motion.x >= mainVars->highscoresNextLT.x && e->motion.x <= mainVars->highscoresNextRB.x && e->motion.y >= mainVars->highscoresNextLT.y && e->motion.y <= mainVars->highscoresNextRB.y)
            {
                mainVars->highscoresNextHover = true;
            }
            else
            {
                mainVars->highscoresNextHover = false;
            }

            // back text
            if (e->motion.x >= mainVars->highscoresBackLT.x && e->motion.x <= mainVars->highscoresBackRB.x && e->motion.y >= mainVars->highscoresBackLT.y && e->motion.y <= mainVars->highscoresBackRB.y)
            {
                mainVars->highscoresBackHover = true;
            }
            else
            {
                mainVars->highscoresBackHover = false;
            }
        }
        else if (windowProperties->currentMenu == LevelSelect)
        {
            // up button
            if (e->motion.x >= mainVars->levelSelectPrevLT.x && e->motion.x <= mainVars->levelSelectPrevRB.x && e->motion.y >= mainVars->levelSelectPrevLT.y && e->motion.y <= mainVars->levelSelectPrevRB.y)
            {
                mainVars->levelSelectPrevHover = true;
            }
            else
            {
                mainVars->levelSelectPrevHover = false;
            }

            // down button
            if (e->motion.x >= mainVars->levelSelectNextLT.x && e->motion.x <= mainVars->levelSelectNextRB.x && e->motion.y >= mainVars->levelSelectNextLT.y && e->motion.y <= mainVars->levelSelectNextRB.y)
            {
                mainVars->levelSelectNextHover = true;
            }
            else
            {
                mainVars->levelSelectNextHover = false;
            }

            // back button
            if (e->motion.x >= mainVars->levelSelectBackLT.x && e->motion.x <= mainVars->levelSelectBackRB.x && e->motion.y >= mainVars->levelSelectBackLT.y && e->motion.y <= mainVars->levelSelectBackRB.y)
            {
                mainVars->levelSelectBackHover = true;
            }
            else
            {
                mainVars->levelSelectBackHover = false;
            }
        }
    }

    if (e->type == SDL_MOUSEBUTTONDOWN)
    {
        if (windowProperties->currentMenu == MainMenu)
        {
            // play
            if (mainVars->mainMenuPlayHover)
            {
                windowProperties->currentMenu = LevelSelect;
                mainVars->mainMenuPlayHover = false;
            }
            // settings
            if (mainVars->mainMenuSettingsHover)
            {
                windowProperties->currentMenu = Settings;
                mainVars->mainMenuSettingsHover = false;
            }

            // highscore
            if (mainVars->mainMenuHighscoresHover)
            {
                windowProperties->currentMenu = Highscore;
                mainVars->mainMenuHighscoresHover = false;
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

                mainVars->settingsScaleHover = false;
            }

            // back
            if (mainVars->settingsBackHover)
            {
                windowProperties->currentMenu = MainMenu;
                mainVars->settingsBackHover = false;
            }
        }
        else if (windowProperties->currentMenu == Highscore)
        {
            // prev button
            if (mainVars->highscoresPrevHover)
            {
                if (mainVars->highscoresOffset > 0)
                    // remove 1 from offset
                    mainVars->highscoresOffset--;

                // check if offset is 0
                if (mainVars->highscoresOffset == 0)
                {
                    // hide button
                    mainVars->highscoresUpButton = false;
                    // stop hover
                    mainVars->highscoresPrevHover = false;
                }

                // if next button is not active
                if (!mainVars->highscoresDownButton)
                {
                    // activate next button
                    mainVars->highscoresDownButton = true;
                }
            }

            // next button
            if (mainVars->highscoresNextHover)
            {
                if (mainVars->highscoresOffset < windowProperties->highscores->count / HIGHSCORES_PER_PAGE)
                    // add 1 to offset
                    mainVars->highscoresOffset++;

                // check if current page have less or equal to HIGHSCORES_PER_PAGE
                if (windowProperties->highscores->count - mainVars->highscoresOffset * HIGHSCORES_PER_PAGE <= HIGHSCORES_PER_PAGE)
                {
                    // hide button
                    mainVars->highscoresDownButton = false;
                    // stop hover
                    mainVars->highscoresNextHover = false;
                }

                // if prev button is not active
                if (!mainVars->highscoresUpButton)
                {
                    // activate prev button
                    mainVars->highscoresUpButton = true;
                }
            }

            // back
            if (mainVars->highscoresBackHover)
            {
                windowProperties->currentMenu = MainMenu;
                mainVars->highscoresBackHover = false;
            }
        }
        else if (windowProperties->currentMenu == LevelSelect)
        {
            // back button
            if (mainVars->levelSelectBackHover)
            {
                windowProperties->currentMenu = MainMenu;
                mainVars->levelSelectBackHover = false;
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