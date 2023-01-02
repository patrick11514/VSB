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

    // FPS
    int FPSHeight = 16;
    if (!renderText(renderer, windowProperties->currentFPS, windowProperties->font, windowProperties->colors->white, 50, FPSHeight, 0, WINDOW_HEIGHT * scale - FPSHeight))
    {
        fprintf(stderr, "Error rendering text: %s\n", SDL_GetError());
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
    else if (windowProperties->currentMenu == LevelInfo)
    {
        renderLevelInfo(renderer, windowProperties, mainVars);
    }
    else if (windowProperties->currentMenu == Game)
    {
        renderGame(renderer, windowProperties, mainVars);
    }
    else if (windowProperties->currentMenu == GameOver)
    {
        renderGameOver(renderer, windowProperties, mainVars);
    }
    else if (windowProperties->currentMenu == Controls)
    {
        renderControls(renderer, windowProperties, mainVars);
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
    float playWidth = 150 * scale;
    float playHeight = 75 * scale;

    float playX = (WINDOW_WIDTH * scale / 2) - (playWidth / 2);
    // title + titleHeight + 50px
    float playY = titleCoords.y + titleCoords.height + 50 * scale;
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
        fprintf(stderr, "Error rendering text: %s\n", SDL_GetError());
    }

    // settings text
    float settingsWidth = 225 * scale;
    float settingsHeight = 75 * scale;

    float settingsX = (WINDOW_WIDTH * scale / 2) - (settingsWidth / 2);
    // play + playHeight + 50px
    float settingsY = playY + playHeight + 25 * scale;
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
        fprintf(stderr, "Error rendering text: %s\n", SDL_GetError());
    }

    // highscores text
    float highscoresWidth = 300 * scale;
    float highscoresHeight = 75 * scale;

    float highscoresX = (WINDOW_WIDTH * scale / 2) - (highscoresWidth / 2);
    // settings + settingsHeight + 50px
    float highscoresY = settingsY + settingsHeight + 25 * scale;
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
        fprintf(stderr, "Error rendering text: %s\n", SDL_GetError());
    }

    // controls menu
    float controlsWidth = 300 * scale;
    float controlsHeight = 75 * scale;

    float controlsX = (WINDOW_WIDTH * scale / 2) - (controlsWidth / 2);
    // highscores + highscoreHeight + 50px
    float controlsY = highscoresY + highscoresHeight + 25 * scale;
    mainVars->mainMenuControlsLT.x = controlsX;
    mainVars->mainMenuControlsLT.y = controlsY;
    mainVars->mainMenuControlsRB.x = controlsX + controlsWidth;
    mainVars->mainMenuControlsRB.y = controlsY + controlsHeight;

    SDL_Color controlsColor = windowProperties->colors->white;
    if (mainVars->mainMenuControlsHover)
    {
        controlsColor = windowProperties->colors->orange;
    }

    if (!renderText(renderer, "CONTROLS", windowProperties->font, controlsColor, controlsWidth, controlsHeight, controlsX, controlsY))
    {
        fprintf(stderr, "Error rendering text: %s\n", SDL_GetError());
    }

    // exit text
    float exitWidth = 100 * scale;
    float exitHeight = 75 * scale;

    float exitX = (WINDOW_WIDTH * scale / 2) - (exitWidth / 2);
    // controls + controlsHeight + 50px
    float exitY = controlsY + controlsHeight + 25 * scale;
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
        fprintf(stderr, "Error rendering text: %s\n", SDL_GetError());
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
    float settingsWidth = 225 * scale;
    float settingsHeight = 75 * scale;

    float settingsX = (WINDOW_WIDTH * scale / 2) - (settingsWidth / 2);
    // title + titleHeight + 25px
    float settingsY = titleCoords.y + titleCoords.height + 25;

    if (!renderText(renderer, "SETTINGS", windowProperties->font, windowProperties->colors->white, settingsWidth, settingsHeight, settingsX, settingsY))
    {
        fprintf(stderr, "Error rendering text: %s\n", SDL_GetError());
    }

    // line under settings
    if (!renderRect(renderer, settingsX, settingsY + settingsHeight, settingsWidth, 3, windowProperties->colors->white))
    {
        fprintf(stderr, "Error rendering rect: %s\n", SDL_GetError());
    }

    // scale text
    float scaleWidth = 325 * scale;
    if (scale >= 1.0)
    {
        scaleWidth = 350 * scale;
    }
    float scaleHeight = 50 * scale;

    float scaleTextX = (WINDOW_WIDTH * scale / 2) - (scaleWidth / 2);
    // settings + settingsHeight + 50px
    float scaleTextY = settingsY + settingsHeight + 50 * scale;
    mainVars->settingsScaleLT.x = scaleTextX;
    mainVars->settingsScaleLT.y = scaleTextY;
    mainVars->settingsScaleRB.x = scaleTextX + scaleWidth;
    mainVars->settingsScaleRB.y = scaleTextY + scaleHeight;

    char scaleText[50];

    snprintf(scaleText, 50, "Current scale: %d%%", (int)(scale * 100));

    SDL_Color scaleColor = windowProperties->colors->white;
    if (mainVars->settingsScaleHover)
    {
        scaleColor = windowProperties->colors->orange;
    }

    if (!renderText(renderer, scaleText, windowProperties->font, scaleColor, scaleWidth, scaleHeight, scaleTextX, scaleTextY))
    {
        fprintf(stderr, "Error rendering text: %s\n", SDL_GetError());
    }

    // back text
    float backWidth = 125 * scale;
    float backHeight = 65 * scale;

    float backX = (WINDOW_WIDTH * scale / 2) - (backWidth / 2);
    // scale + scaleHeight + 50px
    float backY = scaleTextY + scaleHeight + 50 * scale;
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
        fprintf(stderr, "Error rendering text: %s\n", SDL_GetError());
    }
}

void initializeHighscores(Highscores *highscores, WindowProperties *windowProperties, MainVariables *mainVars)
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
    float highscoreWidth = 300 * scale;
    float highscoreHeight = 75 * scale;

    float highscoreX = (WINDOW_WIDTH * scale / 2) - (highscoreWidth / 2);
    // title + titleHeight + 25px
    float highscoreY = titleCoords.y + titleCoords.height + 25;

    if (!renderText(renderer, "HIGHSCORES", windowProperties->font, windowProperties->colors->white, highscoreWidth, highscoreHeight, highscoreX, highscoreY))
    {
        fprintf(stderr, "Error rendering text: %s\n", SDL_GetError());
    }

    // line under highscores
    if (!renderRect(renderer, highscoreX, highscoreY + highscoreHeight, highscoreWidth, 3, windowProperties->colors->white))
    {
        fprintf(stderr, "Error rendering rect: %s\n", SDL_GetError());
    }

    Highscores *highscores = windowProperties->highscores;

    // Y coord for back button edited in if statement
    float backY;
    float backHeight = 65 * scale;

    if (!highscores)
    {
        initializeHighscores(highscores, windowProperties, mainVars);
        highscores = windowProperties->highscores;
    }

    if (highscores->count == 0)
    {
        // no highscores
        float noHighscoresWidth = 300 * scale;
        float noHighscoresHeight = 50 * scale;

        float noHighscoresX = (WINDOW_WIDTH * scale / 2) - (noHighscoresWidth / 2);
        // highscore + highscoreHeight + 50px
        float noHighscoresY = highscoreY + highscoreHeight + 50 * scale;

        if (!renderText(renderer, "No highscores yet", windowProperties->font, windowProperties->colors->red, noHighscoresWidth, noHighscoresHeight, noHighscoresX, noHighscoresY))
        {
            fprintf(stderr, "Error rendering text: %s\n", SDL_GetError());
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

        float buttonUpX = (WINDOW_WIDTH * scale / 2) - (buttonUp->width * scale / 2);
        // highscore + highscoreHeight + 20px (spacing)
        float buttonUpY = highscoreY + highscoreHeight + 20;

        // if enabled render up button
        if (mainVars->highscoresUpButton)
        {

            mainVars->highscoresPrevLT.x = buttonUpX;
            mainVars->highscoresPrevLT.y = buttonUpY;
            mainVars->highscoresPrevRB.x = buttonUpX + buttonUp->width * scale;
            mainVars->highscoresPrevRB.y = buttonUpY + buttonUp->height * scale;

            if (!renderTexture(renderer, buttonUp->texture, buttonUpX, buttonUpY, buttonUp->width * scale, buttonUp->height * scale))
            {
                fprintf(stderr, "Error rendering texture: %s\n", SDL_GetError());
            }
        }

        // highscores
        // buttonUp + buttonUpHeight + 10px
        float highscoreY = buttonUpY + buttonUp->height * scale + 10 * scale;

        float end = mainVars->highscoresOffset * HIGHSCORES_PER_PAGE + HIGHSCORES_PER_PAGE;

        if (end > highscores->count)
        {
            end = highscores->count;
        }

        for (int i = mainVars->highscoresOffset * HIGHSCORES_PER_PAGE; i < end; i++)
        {
            char text[255];

            snprintf(text, 255, "%s - %s", (char *)arrayGet(highscores->players, i), (char *)arrayGet(highscores->scores, i));

            int len = count_utf8_code_points(text);

            // 20px per char
            float x = (WINDOW_WIDTH * scale / 2) - (len * 20 * scale / 2);

            if (!renderText(renderer, text, windowProperties->font, windowProperties->colors->white, len * 20 * scale, 50 * scale, x, highscoreY))
            {
                fprintf(stderr, "Error rendering text: %s\n", SDL_GetError());
            }

            // 50px per highscore
            highscoreY += 50 * scale;
        }

        float buttonDownX = (WINDOW_WIDTH * scale / 2) - (buttonDown->width * scale / 2);
        // highscoreY - 50px (because of last highscore add 50px) + 10px (spacing) + buttonDownHeight
        float buttonDownY = highscoreY - 50 + 10 + buttonDown->height;

        // if enabled render down button
        if (mainVars->highscoresDownButton)
        {

            mainVars->highscoresNextLT.x = buttonDownX;
            mainVars->highscoresNextLT.y = buttonDownY;
            mainVars->highscoresNextRB.x = buttonDownX + buttonDown->width * scale;
            mainVars->highscoresNextRB.y = buttonDownY + buttonDown->height * scale;

            if (!renderTexture(renderer, buttonDown->texture, buttonDownX, buttonDownY, buttonDown->width * scale, buttonDown->height * scale))
            {
                fprintf(stderr, "Error rendering texture: %s\n", SDL_GetError());
            }
        }

        // back button
        //  buttonDown + buttonDownHeight + 20px (spacing)
        backY = buttonDownY + buttonDown->height * scale + 20 * scale;
    }

    float backWidth = 125 * scale;
    float backX = (WINDOW_WIDTH * scale / 2) - (backWidth / 2);

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
        fprintf(stderr, "Error rendering text: %s\n", SDL_GetError());
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
    float levelWidth = 375 * scale;
    float levelHeight = 85 * scale;

    float levelX = (WINDOW_WIDTH * scale / 2) - (levelWidth / 2);
    float levelY = titleCoords.y + titleCoords.height + 10 * scale;

    if (!renderText(renderer, "LEVEL SELECT", windowProperties->font, windowProperties->colors->white, levelWidth, levelHeight, levelX, levelY))
    {
        fprintf(stderr, "Error rendering text: %s\n", SDL_GetError());
    }

    // line under level select
    if (!renderRect(renderer, levelX, levelY + levelHeight, levelWidth, 3, windowProperties->colors->white))
    {
        fprintf(stderr, "Error rendering rect: %s\n", SDL_GetError());
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
    float upButtonY = levelY + levelHeight + 10 * scale;

    if (mainVars->levelSelectUpButton)
    {
        SDL_Texture *texture = windowProperties->textures->buttonUp->texture;
        if (mainVars->levelSelectPrevHover)
        {
            texture = windowProperties->textures->buttonUpHover->texture;
        }

        float upButtonX = (WINDOW_WIDTH * scale / 2) - (windowProperties->textures->buttonUp->width * scale / 2);

        mainVars->levelSelectPrevLT.x = upButtonX;
        mainVars->levelSelectPrevLT.y = upButtonY;
        mainVars->levelSelectPrevRB.x = upButtonX + windowProperties->textures->buttonUp->width * scale;
        mainVars->levelSelectPrevRB.y = upButtonY + windowProperties->textures->buttonUp->height * scale;

        if (!renderTexture(renderer, texture, upButtonX, upButtonY, windowProperties->textures->buttonUp->width * scale, windowProperties->textures->buttonUp->height * scale))
        {
            fprintf(stderr, "Error rendering texture: %s\n", SDL_GetError());
        }
    }

    // render levels
    float end = mainVars->levelSelectOffset * LEVELS_PER_PAGE + LEVELS_PER_PAGE;

    if (end > levels->size)
    {
        end = levels->size;
    }

    float levelTextY = upButtonY + windowProperties->textures->buttonUp->height * scale + 10 * scale;

    if (levels->size == 0)
    {
        // No levels found text
        float noLevelsWidth = 300 * scale;
        float noLevelsHeight = 50 * scale;

        float noLevelsX = (WINDOW_WIDTH * scale / 2) - (noLevelsWidth / 2);
        float noLevelsY = levelTextY;

        if (!renderText(renderer, "No levels found", windowProperties->font, windowProperties->colors->red, noLevelsWidth, noLevelsHeight, noLevelsX, noLevelsY))
        {
            fprintf(stderr, "Error rendering text: %s\n", SDL_GetError());
        }
    }
    else
    {
        for (int i = mainVars->levelSelectOffset * LEVELS_PER_PAGE; i < end; i++)
        {
            Level *level = arrayGet(levels, i);

            char *text = level->description;
            int len = count_utf8_code_points(text);

            // 20px per char
            float levelTextX = (WINDOW_WIDTH * scale / 2) - (len * 20 * scale / 2);

            SDL_Color color = windowProperties->colors->white;

            // add data to array
            if (mainVars->levelsTextCoords->size < i + 1)
            {
                TextCoords *coords = malloc(sizeof(TextCoords));
                coords->x = levelTextX;
                coords->y = levelTextY;
                coords->width = len * 20 * scale;
                coords->height = 50 * scale;
                coords->hover = false;
                if (!arrayAdd(mainVars->levelsTextCoords, coords))
                {
                    fprintf(stderr, "Error adding to array: %s\n", SDL_GetError());
                }
            }
            else
            {
                TextCoords *coords = arrayGet(mainVars->levelsTextCoords, i);
                if (coords->hover)
                {
                    color = windowProperties->colors->orange;
                }
            }

            if (!renderText(renderer, level->description, windowProperties->font, color, len * 20 * scale, 50 * scale, levelTextX, levelTextY))
            {
                fprintf(stderr, "Error rendering text: %s\n", SDL_GetError());
            }

            // 50px per level
            levelTextY += 50 * scale;
        }
    }

    float downButtonY = levelTextY + 10 * scale;

    if (mainVars->levelSelectDownButton)
    {
        float downButtonX = (WINDOW_WIDTH * scale / 2) - (windowProperties->textures->buttonDown->width * scale / 2);

        SDL_Texture *texture = windowProperties->textures->buttonDown->texture;
        if (mainVars->levelSelectNextHover)
        {
            texture = windowProperties->textures->buttonDownHover->texture;
        }

        mainVars->levelSelectNextLT.x = downButtonX;
        mainVars->levelSelectNextLT.y = downButtonY;
        mainVars->levelSelectNextRB.x = downButtonX + windowProperties->textures->buttonDown->width * scale;
        mainVars->levelSelectNextRB.y = downButtonY + windowProperties->textures->buttonDown->height * scale;

        if (!renderTexture(renderer, texture, downButtonX, downButtonY, windowProperties->textures->buttonDown->width * scale, windowProperties->textures->buttonDown->height * scale))
        {
            fprintf(stderr, "Error rendering texture: %s\n", SDL_GetError());
        }
    }

    // back text
    float backWidth = 125 * scale;
    float backHeight = 65 * scale;

    float backX = (WINDOW_WIDTH * scale / 2) - (backWidth / 2);
    // scale + scaleHeight + 50px
    float backY = downButtonY + windowProperties->textures->buttonDown->height * scale + 50 * scale;
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
        fprintf(stderr, "Error rendering text: %s\n", SDL_GetError());
    }
}

void renderLevelInfo(SDL_Renderer *renderer, WindowProperties *windowProperties, MainVariables *mainVars)
{
    float scale = windowProperties->scale;

    Level *level = windowProperties->currentLevel;

    // title text
    TextCoords titleCoords;
    titleCoords.width = 350 * scale;
    titleCoords.height = 100 * scale;
    titleCoords.x = (WINDOW_WIDTH * scale / 2) - (titleCoords.width / 2);
    titleCoords.y = 0;
    renderTitle(renderer, windowProperties, mainVars, &titleCoords);

    // level info text
    float levelWidth = 350 * scale;
    float levelHeight = 85 * scale;

    float levelX = (WINDOW_WIDTH * scale / 2) - (levelWidth / 2);
    float levelY = titleCoords.y + titleCoords.height + 10 * scale;

    if (!renderText(renderer, "LEVEL INFO", windowProperties->font, windowProperties->colors->white, levelWidth, levelHeight, levelX, levelY))
    {
        fprintf(stderr, "Error rendering text: %s\n", SDL_GetError());
    }

    // line under level select
    if (!renderRect(renderer, levelX, levelY + levelHeight, levelWidth, 3, windowProperties->colors->white))
    {
        fprintf(stderr, "Error rendering rect: %s\n", SDL_GetError());
    }

    // level name
    // 20px per char
    int levelLen = count_utf8_code_points(level->description);

    float levelNameWidth = levelLen * 20 * scale;
    float levelNameHeight = 50 * scale;

    float levelNameX = (WINDOW_WIDTH * scale / 2) - (levelNameWidth / 2);
    float levelNameY = levelY + levelHeight + 10 * scale;

    if (!renderText(renderer, level->description, windowProperties->font, windowProperties->colors->white, levelNameWidth, levelNameHeight, levelNameX, levelNameY))
    {
        fprintf(stderr, "Error rendering text: %s\n", SDL_GetError());
    }

    // health
    char health[20];
    snprintf(health, 20, "HEALTH: %d", level->health);
    int healthLen = count_utf8_code_points(health);

    float healthWidth = healthLen * 20 * scale;
    float healthHeight = 50 * scale;

    // len of text + width of heart + 10px spacing
    float healthX = (WINDOW_WIDTH * scale / 2) - ((healthWidth + windowProperties->textures->heart->width + 10) / 2);
    float healthY = levelNameY + levelNameHeight * scale;

    if (!renderText(renderer, health, windowProperties->font, windowProperties->colors->white, healthWidth, healthHeight, healthX, healthY))
    {
        fprintf(stderr, "Error rendering text: %s\n", SDL_GetError());
    }

    // heart
    // 10px spacing
    float heartX = healthX + healthWidth + 10;
    float heartY = healthY;

    if (!renderTexture(renderer, windowProperties->textures->heart->texture, heartX, heartY, windowProperties->textures->heart->width * scale, windowProperties->textures->heart->height * scale))
    {
        fprintf(stderr, "Error rendering texture: %s\n", SDL_GetError());
    }

    float brickY = healthY + healthHeight * scale;

    // available bricks
    for (int i = 0; i < level->brickHealths->size; i++)
    {
        BrickHealth *brickHealth = arrayGet(level->brickHealths, i);

        char text[5];
        snprintf(text, 5, "%d", brickHealth->lives);
        int len = count_utf8_code_points(text);

        // Brick texture Text Heart icon
        float brickHeight = 50 * scale;
        float brickTextureScale = brickHeight / (brickHealth->texture->height * scale);
        //  Brick Width + 10px spacing + text width (20px per char) + 10px spacing + heart width
        float brickWidth = (len * 20 + windowProperties->textures->heart->width + 10 * 2) * scale + brickHealth->texture->width * brickTextureScale;

        float brickX = (WINDOW_WIDTH * scale / 2) - (brickWidth / 2);

        // render brick
        // brickY + 5px for centering
        if (!renderTexture(renderer, brickHealth->texture->texture, brickX, brickY + 5, brickHealth->texture->width * brickTextureScale, brickHealth->texture->height * scale * brickTextureScale))
        {
            fprintf(stderr, "Error rendering texture: %s\n", SDL_GetError());
        }

        float textX = brickX + brickHealth->texture->width * brickTextureScale + 10 * scale;

        // render text
        if (!renderText(renderer, text, windowProperties->font, windowProperties->colors->white, len * 20 * scale, brickHeight, textX, brickY))
        {
            fprintf(stderr, "Error rendering text: %s\n", SDL_GetError());
        }

        float heartX = textX + len * 20 * scale + 10 * scale;

        // render heart
        if (!renderTexture(renderer, windowProperties->textures->heart->texture, heartX, brickY, windowProperties->textures->heart->width * scale, windowProperties->textures->heart->height * scale))
        {
            fprintf(stderr, "Error rendering texture: %s\n", SDL_GetError());
        }

        brickY += brickHeight + 10 * scale;
    }

    // start text
    float startWidth = 175 * scale;
    float startHeight = 85 * scale;

    float startX = (WINDOW_WIDTH * scale / 2) - (startWidth / 2);
    float startY = brickY;

    mainVars->levelInfoStartLT.x = startX;
    mainVars->levelInfoStartLT.y = startY;
    mainVars->levelInfoStartRB.x = startX + startWidth;
    mainVars->levelInfoStartRB.y = startY + startHeight;

    SDL_Color startColor = windowProperties->colors->white;

    if (mainVars->levelInfoStartHover)
    {
        startColor = windowProperties->colors->orange;
    }

    if (!renderText(renderer, "START", windowProperties->font, startColor, startWidth, startHeight, startX, startY))
    {
        fprintf(stderr, "Error rendering text: %s\n", SDL_GetError());
    }

    // back text
    float backWidth = 175 * scale;
    float backHeight = 85 * scale;

    float backX = (WINDOW_WIDTH * scale / 2) - (backWidth / 2);
    float backY = startY + startHeight * scale;

    mainVars->levelInfoBackLT.x = backX;
    mainVars->levelInfoBackLT.y = backY;
    mainVars->levelInfoBackRB.x = backX + backWidth;
    mainVars->levelInfoBackRB.y = backY + backHeight;

    SDL_Color backColor = windowProperties->colors->white;

    if (mainVars->levelInfoBackHover)
    {
        backColor = windowProperties->colors->orange;
    }

    if (!renderText(renderer, "BACK", windowProperties->font, backColor, backWidth, backHeight, backX, backY))
    {
        fprintf(stderr, "Error rendering text: %s\n", SDL_GetError());
    }
}

void renderGame(SDL_Renderer *renderer, WindowProperties *windowProperties, MainVariables *mainVars)
{
    float scale = windowProperties->scale;

    // line at top
    float topPadding = 120 * scale;

    if (!renderRect(renderer, 0, topPadding, WINDOW_WIDTH * scale, 3 * scale, windowProperties->colors->white))
    {
        fprintf(stderr, "Error rendering rect: %s\n", SDL_GetError());
    }

    /**
     * Concept:
     * Numbers will be actually rendered by text and heart will be texture
     * ----------------------------------------
     * |     Score        |       Lives       |
     * |     0000         |    5555   0 0     |
     * |     0  0         |    5     0 0 0    |
     * |     0  0         |    555   0   0    |
     * |     0  0         |      5    0 0     |
     * |     0000         |    555     0      |
     * ----------------------------------------
     */

    // vertical line
    if (!renderRect(renderer, WINDOW_WIDTH * scale / 2 - 1.5, 0, 3, topPadding, windowProperties->colors->white))
    {
        fprintf(stderr, "Error rendering rect: %s\n", SDL_GetError());
    }

    // score text
    float scoreTextWidth = 80 * scale;
    float scoreTextHeight = 35 * scale;

    float scoreTextX = (WINDOW_WIDTH * scale / 4) - (scoreTextWidth / 2);
    float scoreTextY = 0;

    if (!renderText(renderer, "Score", windowProperties->font, windowProperties->colors->white, scoreTextWidth, scoreTextHeight, scoreTextX, scoreTextY))
    {
        fprintf(stderr, "Error rendering text: %s\n", SDL_GetError());
    }

    // score number
    char scoreText[5];
    snprintf(scoreText, 5, "%d", windowProperties->score);
    int scoreLen = count_utf8_code_points(scoreText);

    // 20px per char
    float scoreWidth = scoreLen * 20 * scale;
    float scoreHeight = 50 * scale;

    float scoreX = (WINDOW_WIDTH * scale / 4) - (scoreWidth / 2);
    // 120 is height of space between top and line
    float scoreY = ((120 * scale - scoreTextHeight) / 2) - (scoreHeight / 2) + scoreTextHeight;

    if (!renderText(renderer, scoreText, windowProperties->font, windowProperties->colors->white, scoreWidth, scoreHeight, scoreX, scoreY))
    {
        fprintf(stderr, "Error rendering text: %s\n", SDL_GetError());
    }

    // lives text
    float livesTextWidth = 80 * scale;
    float livesTextHeight = 35 * scale;

    float livesTextX = (WINDOW_WIDTH * scale / 4) * 3 - (livesTextWidth / 2);
    float livesTextY = 0;

    if (!renderText(renderer, "Lives", windowProperties->font, windowProperties->colors->white, livesTextWidth, livesTextHeight, livesTextX, livesTextY))
    {
        fprintf(stderr, "Error rendering text: %s\n", SDL_GetError());
    }

    // lives number + heart texture
    char livesText[5];
    snprintf(livesText, 5, "%d", windowProperties->lives);
    int len = count_utf8_code_points(livesText);

    // 20px per char
    float livesWidth = len * 20 * scale;
    float livesHeight = 35 * scale;

    Texture *hearthTexture = windowProperties->textures->heart;

    // scaling for heart texture
    float scaling = (float)livesHeight / hearthTexture->height;

    //+ hearth + spacing 10px
    float livesX = (WINDOW_WIDTH * scale / 4) * 3 - (livesWidth + windowProperties->textures->heart->width * scaling / 2) + 10 * scale;
    float livesY = ((120 * scale - livesTextHeight) / 2) - (livesHeight / 2) + livesTextHeight;

    if (!renderText(renderer, livesText, windowProperties->font, windowProperties->colors->white, livesWidth, livesHeight, livesX, livesY))
    {
        fprintf(stderr, "Error rendering text: %s\n", SDL_GetError());
    }

    // heart texture
    if (!renderTexture(renderer, hearthTexture->texture, livesX + livesWidth + 10 * scale, livesY, hearthTexture->width * scaling, hearthTexture->height * scaling))
    {
        fprintf(stderr, "Error rendering texture: %s\n", SDL_GetError());
    }

    //******************** GAME ********************

    // ball with 0.66 scale
    if (!renderTexture(renderer, windowProperties->textures->ball->texture, windowProperties->ballX, windowProperties->ballY, windowProperties->textures->ball->width * scale * 0.66, windowProperties->textures->ball->height * scale * 0.66))
    {
        fprintf(stderr, "Error rendering texture: %s\n", SDL_GetError());
    }

    // paddle with scale 3
    if (!renderTexture(renderer, windowProperties->textures->paddle->texture, windowProperties->paddleX, windowProperties->paddleY, windowProperties->textures->paddle->width * scale * 3, windowProperties->textures->paddle->height * scale * 3))
    {
        fprintf(stderr, "Error rendering texture: %s\n", SDL_GetError());
    }

    // rectangles for paddle and ball
    SDL_Rect paddleRect = {windowProperties->paddleX, windowProperties->paddleY, windowProperties->textures->paddle->width * scale * 3, windowProperties->textures->paddle->height * scale * 3};
    SDL_Rect ballRect = {windowProperties->ballX, windowProperties->ballY, windowProperties->textures->ball->width * scale * 0.66, windowProperties->textures->ball->height * scale * 0.66};

    // ball
    float ballX = windowProperties->ballX;
    float ballY = windowProperties->ballY;
    // check X position

    ballX += windowProperties->ballSpeedX * (MOVE_FPS / mainVars->FPS) * windowProperties->ballSpeedModifier;
    ballY += windowProperties->ballSpeedY * (MOVE_FPS / mainVars->FPS) * windowProperties->ballSpeedModifier;

    if (windowProperties->ballSpeedModifier < windowProperties->currentLevel->ballSpeedMax)
    {
        windowProperties->ballSpeedModifier += (MOVE_FPS / mainVars->FPS) * windowProperties->currentLevel->ballSpeedModifier;
    }

    // if hit right wall
    if (ballX + windowProperties->textures->ball->width * scale * 0.66 > WINDOW_WIDTH * scale)
    {
        ballX = WINDOW_WIDTH * scale - windowProperties->textures->ball->width * scale * 0.66;

        windowProperties->ballSpeedX = -1 * windowProperties->ballSpeedX;
    }

    // if hit top wall
    // 120px is line under texts (score, lives)
    if (ballY < 120 * scale)
    {
        ballY = 120 * scale;

        windowProperties->ballSpeedY = -1 * windowProperties->ballSpeedY;
    }

    // if hit left wall
    if (ballX < 0)
    {
        ballX = 0;

        windowProperties->ballSpeedX = -1 * windowProperties->ballSpeedX;
    }

    // if hit bottom wall
    // remove 1 life
    // reset ball position and speedX speedY
    if (ballY > WINDOW_HEIGHT * scale)
    {
        windowProperties->lives--;

        // ball will start on paddle
        ballX = windowProperties->paddleX + windowProperties->textures->paddle->width * scale * 3 / 2 - windowProperties->textures->ball->width * scale * 0.66 / 2;
        ballY = windowProperties->paddleY - windowProperties->textures->ball->height * scale * 0.66;

        // default ball direction is top-right
        windowProperties->ballSpeedX = 1;
        windowProperties->ballSpeedY = -1;
        windowProperties->ballSpeedModifier = windowProperties->currentLevel->ballSpeed;
    }

    if (SDL_HasIntersection(&ballRect, &paddleRect))
    {
        ballY = windowProperties->paddleY - windowProperties->textures->ball->height * scale * 0.66;

        windowProperties->ballSpeedY = -1 * windowProperties->ballSpeedY;
    }

    windowProperties->ballX = ballX;
    windowProperties->ballY = ballY;

    Array *bricks = windowProperties->currentLevel->bricks;

    // 40 spacing from line which is 120px from top
    float brickY = (120 + 40) * scale;

    bool alreadyIntersected = false;
    bool finishedGame = true;

    // bricks
    // scale of brick is 1.3
    float brickScale = 1.3;
    for (int i = 0; i < bricks->size; i++)
    {
        Array *brickLine = (Array *)arrayGet(bricks, i);
        // each brick have same width and height
        float startX = (WINDOW_WIDTH * scale - brickLine->size * windowProperties->textures->brickYellow->width * scale * brickScale) / 2;

        for (int l = 0; l < brickLine->size; l++)
        {
            Brick *brick = (Brick *)arrayGet(brickLine, l);
            // if brick is not destroyed
            Texture *texture = brick->texture;

            float width = texture->width * brickScale * scale;
            float height = texture->height * brickScale * scale;

            if (!brick->destroyed)
            {
                finishedGame = false;
                // HANDLE INTERSECTIONS WITH BALL
                if (!alreadyIntersected)
                {

                    SDL_Rect brickTopRect = {startX, brickY, width, 1};
                    SDL_Rect brickBottomRect = {startX, brickY + height, width, 1};
                    SDL_Rect brickLeftRect = {startX, brickY, 1, height};
                    SDL_Rect brickRightRect = {startX + width, brickY, 1, height};

                    int intersected = false;

                    // intersection with top of brick
                    if (SDL_HasIntersection(&ballRect, &brickTopRect))
                    {
                        windowProperties->ballSpeedY = -1 * windowProperties->ballSpeedY;
                        // move ball to top of brick
                        windowProperties->ballY = brickY - windowProperties->textures->ball->height * scale * 0.66 - 1;
                        alreadyIntersected = true;
                        intersected = true;
                    }

                    // intersection with bottom of brick
                    else if (SDL_HasIntersection(&ballRect, &brickBottomRect))
                    {
                        windowProperties->ballSpeedY = -1 * windowProperties->ballSpeedY;
                        // move ball to bottom of brick
                        windowProperties->ballY = brickY + height + 1;
                        alreadyIntersected = true;
                        intersected = true;
                    }

                    // intersection with left of brick
                    else if (SDL_HasIntersection(&ballRect, &brickLeftRect))
                    {
                        windowProperties->ballSpeedX = -1 * windowProperties->ballSpeedX;
                        // move ball to left of brick
                        windowProperties->ballX = startX - windowProperties->textures->ball->width * scale * 0.66 + 1;
                        alreadyIntersected = true;
                        intersected = true;
                    }

                    // intersection with right of brick
                    else if (SDL_HasIntersection(&ballRect, &brickRightRect))
                    {
                        windowProperties->ballSpeedX = -1 * windowProperties->ballSpeedX;
                        // move ball to right of brick
                        windowProperties->ballX = startX + width - 1;
                        alreadyIntersected = true;
                        intersected = true;
                    }

                    if (intersected)
                    {
                        // remove live and change texture
                        int newLive = brick->lives - 1;
                        // add score
                        windowProperties->score += windowProperties->currentLevel->scorePerBrick;

                        if (newLive == 0)
                        {
                            brick->destroyed = true;
                        }
                        else
                        {
                            BrickHealth *brickHealth = (BrickHealth *)arrayGet(windowProperties->currentLevel->brickHealths, newLive - 1);
                            brick->texture = brickHealth->texture;
                            brick->lives = newLive;
                            // set brick with new values
                            if (!arraySet(brickLine, brick, l))
                            {
                                fprintf(stderr, "Error setting brick in array: %s\n", SDL_GetError());
                            }
                        }
                    }
                }
                //
                if (!renderTexture(renderer, texture->texture, startX, brickY, width, height))
                {
                    fprintf(stderr, "Error rendering texture: %s\n", SDL_GetError());
                }
            }

            startX += width;
        }

        // set brickLine if some bricks changes
        if (!arraySet(bricks, brickLine, i))
        {
            fprintf(stderr, "Error setting brickLine in array: %s\n", SDL_GetError());
        }

        if (finishedGame)
        {
            windowProperties->currentMenu = GameOver;
        }

        brickY += windowProperties->textures->brickYellow->height * brickScale * scale;
    }

    // update bricks in level
    windowProperties->currentLevel->bricks = bricks;

    // if 0 lives
    if (windowProperties->lives == 0)
    {
        // end game
        windowProperties->currentMenu = GameOver;
    }
}

void renderGameOver(SDL_Renderer *renderer, WindowProperties *windowProperties, MainVariables *mainVars)
{
    float scale = windowProperties->scale;

    Level *level = windowProperties->currentLevel;

    // title text
    TextCoords titleCoords;
    titleCoords.width = 350 * scale;
    titleCoords.height = 100 * scale;
    titleCoords.x = (WINDOW_WIDTH * scale / 2) - (titleCoords.width / 2);
    titleCoords.y = 0;
    renderTitle(renderer, windowProperties, mainVars, &titleCoords);

    // level info text
    float levelWidth = 350 * scale;
    float levelHeight = 85 * scale;

    float levelX = (WINDOW_WIDTH * scale / 2) - (levelWidth / 2);
    float levelY = titleCoords.y + titleCoords.height + 10 * scale;

    if (!renderText(renderer, (windowProperties->lives == 0) ? "YOU LOST" : "YOU WON", windowProperties->font, windowProperties->colors->white, levelWidth, levelHeight, levelX, levelY))
    {
        fprintf(stderr, "Error rendering text: %s\n", SDL_GetError());
    }

    // line under level select
    if (!renderRect(renderer, levelX, levelY + levelHeight, levelWidth, 3, windowProperties->colors->white))
    {
        fprintf(stderr, "Error rendering rect: %s\n", SDL_GetError());
    }

    // level name
    // 20px per char
    int levelLen = count_utf8_code_points(level->description);

    float levelNameWidth = levelLen * 20 * scale;
    float levelNameHeight = 50 * scale;

    float levelNameX = (WINDOW_WIDTH * scale / 2) - (levelNameWidth / 2);
    float levelNameY = levelY + levelHeight + 10 * scale;

    if (!renderText(renderer, level->description, windowProperties->font, windowProperties->colors->white, levelNameWidth, levelNameHeight, levelNameX, levelNameY))
    {
        fprintf(stderr, "Error rendering text: %s\n", SDL_GetError());
    }

    // health
    char health[20];
    snprintf(health, 20, "HEALTH: %d", windowProperties->lives);
    int healthLen = count_utf8_code_points(health);

    float healthWidth = healthLen * 20 * scale;
    float healthHeight = 50 * scale;

    // len of text + width of heart + 10px spacing
    float healthX = (WINDOW_WIDTH * scale / 2) - ((healthWidth + windowProperties->textures->heart->width + 10) / 2);
    float healthY = levelNameY + levelNameHeight * scale;

    if (!renderText(renderer, health, windowProperties->font, windowProperties->colors->white, healthWidth, healthHeight, healthX, healthY))
    {
        fprintf(stderr, "Error rendering text: %s\n", SDL_GetError());
    }

    // heart
    // 10px spacing
    float heartX = healthX + healthWidth + 10;
    float heartY = healthY;

    if (!renderTexture(renderer, windowProperties->textures->heart->texture, heartX, heartY, windowProperties->textures->heart->width * scale, windowProperties->textures->heart->height * scale))
    {
        fprintf(stderr, "Error rendering texture: %s\n", SDL_GetError());
    }

    // score
    char score[20];
    snprintf(score, 20, "SCORE: %d", windowProperties->score);
    int scoreLen = count_utf8_code_points(score);

    float scoreWidth = scoreLen * 20 * scale;
    float scoreHeight = 50 * scale;

    float scoreX = (WINDOW_WIDTH * scale / 2) - (scoreWidth / 2);
    float scoreY = healthY + healthHeight + 10 * scale;

    if (!renderText(renderer, score, windowProperties->font, windowProperties->colors->white, scoreWidth, scoreHeight, scoreX, scoreY))
    {
        fprintf(stderr, "Error rendering text: %s\n", SDL_GetError());
    }

    // enter username text
    float enterUsernameWidth = 350 * scale;
    float enterUsernameHeight = 50 * scale;

    float enterUsernameX = (WINDOW_WIDTH * scale / 2) - (enterUsernameWidth / 2);
    float enterUsernameY = scoreY + scoreHeight + 10 * scale;

    if (!renderText(renderer, "ENTER USERNAME:", windowProperties->font, windowProperties->colors->white, enterUsernameWidth, enterUsernameHeight, enterUsernameX, enterUsernameY))
    {
        fprintf(stderr, "Error rendering text: %s\n", SDL_GetError());
    }

    // username text
    char *text = windowProperties->currentUserName;
    int textLen = count_utf8_code_points(text);

    // 20px per char
    float usernameWidth = textLen * 20 * scale;
    float usernameHeight = 50 * scale;

    float usernameX = (WINDOW_WIDTH * scale / 2) - (usernameWidth / 2);
    float usernameY = enterUsernameY + enterUsernameHeight + 10 * scale;

    if (textLen > 0)
    {
        if (!renderText(renderer, text, windowProperties->font, windowProperties->colors->white, usernameWidth, usernameHeight, usernameX, usernameY))
        {
            fprintf(stderr, "Error rendering text: %s\n", SDL_GetError());
        }
    }

    // line under username - like input - so 20 chars is max => 20 * 20px
    float usernameLineWidth = 20 * 20 * scale;
    float usernameLineHeight = 3 * scale;

    float usernameLineX = (WINDOW_WIDTH * scale / 2) - (usernameLineWidth / 2);
    float usernameLineY = usernameY + usernameHeight + 10 * scale;

    if (!renderRect(renderer, usernameLineX, usernameLineY, usernameLineWidth, usernameLineHeight, windowProperties->colors->white))
    {
        fprintf(stderr, "Error rendering rect: %s\n", SDL_GetError());
    }

    // max 20 chars text
    float maxCharsWidth = 175 * scale;
    float maxCharsHeight = 30 * scale;

    float maxCharsX = (WINDOW_WIDTH * scale / 2) - (maxCharsWidth / 2);
    float maxCharsY = usernameLineY + usernameLineHeight + 10 * scale;

    if (!renderText(renderer, "Max 20 characters", windowProperties->font, windowProperties->colors->red, maxCharsWidth, maxCharsHeight, maxCharsX, maxCharsY))
    {
        fprintf(stderr, "Error rendering text: %s\n", SDL_GetError());
    }

    // submit highscore text
    float submitHighscoreWidth = 350 * scale;
    float submitHighscoreHeight = 50 * scale;

    float submitHighscoreX = (WINDOW_WIDTH * scale / 2) - (submitHighscoreWidth / 2);
    float submitHighscoreY = maxCharsY + maxCharsHeight + 10 * scale;

    mainVars->gameOverSaveLT.x = submitHighscoreX;
    mainVars->gameOverSaveLT.y = submitHighscoreY;
    mainVars->gameOverSaveRB.x = submitHighscoreX + submitHighscoreWidth;
    mainVars->gameOverSaveRB.y = submitHighscoreY + submitHighscoreHeight;

    SDL_Color submitHighscoreColor = windowProperties->colors->white;
    if (mainVars->gameOverSaveHover)
    {
        submitHighscoreColor = windowProperties->colors->orange;
    }

    if (!renderText(renderer, "SUBMIT HIGHSCORE", windowProperties->font, submitHighscoreColor, submitHighscoreWidth, submitHighscoreHeight, submitHighscoreX, submitHighscoreY))
    {
        fprintf(stderr, "Error rendering text: %s\n", SDL_GetError());
    }

    // back to menu text
    float backToMenuWidth = 350 * scale;
    float backToMenuHeight = 50 * scale;

    float backToMenuX = (WINDOW_WIDTH * scale / 2) - (backToMenuWidth / 2);
    float backToMenuY = submitHighscoreY + submitHighscoreHeight + 10 * scale;

    mainVars->gameOverBackLT.x = backToMenuX;
    mainVars->gameOverBackLT.y = backToMenuY;
    mainVars->gameOverBackRB.x = backToMenuX + backToMenuWidth;
    mainVars->gameOverBackRB.y = backToMenuY + backToMenuHeight;

    SDL_Color backToMenuColor = windowProperties->colors->white;
    if (mainVars->gameOverBackHover)
    {
        backToMenuColor = windowProperties->colors->orange;
    }

    if (!renderText(renderer, "BACK TO MENU", windowProperties->font, backToMenuColor, backToMenuWidth, backToMenuHeight, backToMenuX, backToMenuY))
    {
        fprintf(stderr, "Error rendering text: %s\n", SDL_GetError());
    }
}

void renderControls(SDL_Renderer *renderer, WindowProperties *windowProperties, MainVariables *mainVars)
{
    float scale = windowProperties->scale;

    // title text
    TextCoords titleCoords;
    titleCoords.width = 350 * scale;
    titleCoords.height = 100 * scale;
    titleCoords.x = (WINDOW_WIDTH * scale / 2) - (titleCoords.width / 2);
    titleCoords.y = 0;
    renderTitle(renderer, windowProperties, mainVars, &titleCoords);

    // controls text
    float controlsWidth = 275 * scale;
    float controlsHeight = 50 * scale;

    float controlsX = (WINDOW_WIDTH * scale / 2) - (controlsWidth / 2);
    float controlsY = titleCoords.y + titleCoords.height + 10 * scale;

    if (!renderText(renderer, "CONTROLS", windowProperties->font, windowProperties->colors->white, controlsWidth, controlsHeight, controlsX, controlsY))
    {
        fprintf(stderr, "Error rendering text: %s\n", SDL_GetError());
    }

    // ESC - back to menu select
    float escWidth = 600 * scale;
    float escHeight = 50 * scale;

    float escX = (WINDOW_WIDTH * scale / 2) - (escWidth / 2);
    float escY = controlsY + controlsHeight + 10 * scale;

    if (!renderText(renderer, "ESC - back to menu select (works in game)", windowProperties->font, windowProperties->colors->white, escWidth, escHeight, escX, escY))
    {
        fprintf(stderr, "Error rendering text: %s\n", SDL_GetError());
    }

    // A/Arrow left - move paddle left
    float aWidth = 450 * scale;
    float aHeight = 50 * scale;

    float aX = (WINDOW_WIDTH * scale / 2) - (aWidth / 2);
    float aY = escY + escHeight + 10 * scale;

    if (!renderText(renderer, "A/Arrow left - move paddle left", windowProperties->font, windowProperties->colors->white, aWidth, aHeight, aX, aY))
    {
        fprintf(stderr, "Error rendering text: %s\n", SDL_GetError());
    }

    // D/Arrow right - move paddle right
    float dWidth = 475 * scale;
    float dHeight = 50 * scale;

    float dX = (WINDOW_WIDTH * scale / 2) - (dWidth / 2);
    float dY = aY + aHeight + 10 * scale;

    if (!renderText(renderer, "D/Arrow right - move paddle right", windowProperties->font, windowProperties->colors->white, dWidth, dHeight, dX, dY))
    {
        fprintf(stderr, "Error rendering text: %s\n", SDL_GetError());
    }

    // back button
    float backWidth = 125 * scale;
    float backHeight = 50 * scale;

    float backX = (WINDOW_WIDTH * scale / 2) - (backWidth / 2);
    float backY = dY + dHeight + 10 * scale;

    mainVars->controlsBackLT.x = backX;
    mainVars->controlsBackLT.y = backY;
    mainVars->controlsBackRB.x = backX + backWidth;
    mainVars->controlsBackRB.y = backY + backHeight;

    SDL_Color backColor = windowProperties->colors->white;
    if (mainVars->controlsBackHover)
    {
        backColor = windowProperties->colors->orange;
    }

    if (!renderText(renderer, "BACK", windowProperties->font, backColor, backWidth, backHeight, backX, backY))
    {
        fprintf(stderr, "Error rendering text: %s\n", SDL_GetError());
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
    float paddlePathWidth = 250 * scale;
    // paddle is under title + 10 px up
    mainVars->paddlePosition.y = 0 + textCoords->height - 10;
    mainVars->paddleStartPosition = (WINDOW_WIDTH * scale / 2) - (paddlePathWidth / 2);

    float paddleEnd = mainVars->paddleStartPosition + paddlePathWidth - windowProperties->textures->paddle->width * scale;

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
    float scale = windowProperties->scale;

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

            // constrolls text
            if (e->motion.x >= mainVars->mainMenuControlsLT.x && e->motion.x <= mainVars->mainMenuControlsRB.x && e->motion.y >= mainVars->mainMenuControlsLT.y && e->motion.y <= mainVars->mainMenuControlsRB.y)
            {
                mainVars->mainMenuControlsHover = true;
            }
            else
            {
                mainVars->mainMenuControlsHover = false;
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
            // texts
            for (int i = 0; i < mainVars->levelsTextCoords->size; i++)
            {
                TextCoords *textCoords = arrayGet(mainVars->levelsTextCoords, i);

                int leftTopX = textCoords->x;
                int leftTopY = textCoords->y;
                int rightBottomX = textCoords->x + textCoords->width;
                int rightBottomY = textCoords->y + textCoords->height;

                if (e->motion.x >= leftTopX && e->motion.x <= rightBottomX && e->motion.y >= leftTopY && e->motion.y <= rightBottomY)
                {
                    textCoords->hover = true;
                }
                else
                {
                    textCoords->hover = false;
                }
            }

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
        else if (windowProperties->currentMenu == LevelInfo)
        {
            // start button
            if (e->motion.x >= mainVars->levelInfoStartLT.x && e->motion.x <= mainVars->levelInfoStartRB.x && e->motion.y >= mainVars->levelInfoStartLT.y && e->motion.y <= mainVars->levelInfoStartRB.y)
            {
                mainVars->levelInfoStartHover = true;
            }
            else
            {
                mainVars->levelInfoStartHover = false;
            }

            // back button
            if (e->motion.x >= mainVars->levelInfoBackLT.x && e->motion.x <= mainVars->levelInfoBackRB.x && e->motion.y >= mainVars->levelInfoBackLT.y && e->motion.y <= mainVars->levelInfoBackRB.y)
            {
                mainVars->levelInfoBackHover = true;
            }
            else
            {
                mainVars->levelInfoBackHover = false;
            }
        }
        else if (windowProperties->currentMenu == GameOver)
        {
            // save highscore button
            if (e->motion.x >= mainVars->gameOverSaveLT.x && e->motion.x <= mainVars->gameOverSaveRB.x && e->motion.y >= mainVars->gameOverSaveLT.y && e->motion.y <= mainVars->gameOverSaveRB.y)
            {
                mainVars->gameOverSaveHover = true;
            }
            else
            {
                mainVars->gameOverSaveHover = false;
            }

            // back to menu button
            if (e->motion.x >= mainVars->gameOverBackLT.x && e->motion.x <= mainVars->gameOverBackRB.x && e->motion.y >= mainVars->gameOverBackLT.y && e->motion.y <= mainVars->gameOverBackRB.y)
            {
                mainVars->gameOverBackHover = true;
            }
            else
            {
                mainVars->gameOverBackHover = false;
            }
        }
        else if (windowProperties->currentMenu == Controls)
        {
            // back text
            if (e->motion.x >= mainVars->controlsBackLT.x && e->motion.x <= mainVars->controlsBackRB.x && e->motion.y >= mainVars->controlsBackLT.y && e->motion.y <= mainVars->controlsBackRB.y)
            {
                mainVars->controlsBackHover = true;
            }
            else
            {
                mainVars->controlsBackHover = false;
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

            // controlls
            if (mainVars->mainMenuControlsHover)
            {
                windowProperties->currentMenu = Controls;
                mainVars->mainMenuControlsHover = false;
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
                // reset offset
                mainVars->highscoresOffset = 0;
                // reset buttons
                mainVars->highscoresUpButton = false;
                if (windowProperties->highscores->count > HIGHSCORES_PER_PAGE)
                {
                    mainVars->highscoresDownButton = true;
                }
            }
        }
        else if (windowProperties->currentMenu == LevelSelect)
        {
            // text
            for (int i = 0; i < mainVars->levelsTextCoords->size; i++)
            {
                TextCoords *textCoords = arrayGet(mainVars->levelsTextCoords, i);
                if (textCoords->hover)
                {

                    // make copy of bricks, so we can reset level
                    if (windowProperties->currentLevel != NULL)
                    {
                        // free previous level
                        Level *levelData = windowProperties->currentLevel;

                        for (int i = 0; i < levelData->bricks->size; i++)
                        {
                            Array *bricksLine = (Array *)arrayGet(levelData->bricks, i);
                            for (int i = 0; i < bricksLine->size; i++)
                            {
                                Brick *brick = (Brick *)arrayGet(bricksLine, i);
                                free(brick);
                            }
                            if (!arrayFree(bricksLine, true))
                            {
                                fprintf(stderr, "Unable to free brickLine array.\n");
                            }
                        }

                        if (!arrayFree(levelData->bricks, true))
                        {
                            fprintf(stderr, "Unable to free bricks array.\n");
                        }

                        free(levelData);
                    }

                    // allocate new memory for level
                    windowProperties->currentLevel = (Level *)malloc(sizeof(Level));

                    Level *selectedLevel = arrayGet(windowProperties->levels, i + mainVars->levelSelectOffset * LEVELS_PER_PAGE);

                    // copy values
                    windowProperties->currentLevel->ballSpeed = selectedLevel->ballSpeed;
                    windowProperties->currentLevel->ballSpeedMax = selectedLevel->ballSpeedMax;
                    windowProperties->currentLevel->ballSpeedModifier = selectedLevel->ballSpeedModifier;
                    windowProperties->currentLevel->description = selectedLevel->description;
                    windowProperties->currentLevel->health = selectedLevel->health;
                    windowProperties->currentLevel->score = selectedLevel->score;
                    windowProperties->currentLevel->scorePerBrick = selectedLevel->scorePerBrick;
                    windowProperties->currentLevel->brickHealths = selectedLevel->brickHealths;

                    windowProperties->currentLevel->bricks = arrayInit(selectedLevel->bricks->size);
                    // copy bricks
                    for (int i = 0; i < selectedLevel->bricks->size; i++)
                    {
                        Array *bricksLine = (Array *)arrayGet(selectedLevel->bricks, i);
                        Array *newBricksLine = arrayInit(bricksLine->size);
                        for (int l = 0; l < bricksLine->size; l++)
                        {
                            Brick *brick = (Brick *)arrayGet(bricksLine, l);
                            Brick *newBrick = (Brick *)malloc(sizeof(Brick));

                            // copy values
                            newBrick->destroyed = brick->destroyed;
                            newBrick->lives = brick->lives;
                            newBrick->texture = brick->texture;
                            newBrick->x = brick->x;
                            newBrick->y = brick->y;

                            // add to array
                            if (!arrayAdd(newBricksLine, newBrick))
                            {
                                fprintf(stderr, "Unable to add brick to array.\n");
                            }
                        }
                        if (!arrayAdd(windowProperties->currentLevel->bricks, newBricksLine))
                        {
                            fprintf(stderr, "Unable to add brickLine to array.\n");
                        }
                    }

                    windowProperties->currentMenu = LevelInfo;
                    textCoords->hover = false;

                    // after starting level, clear texts from array
                    for (int i = 0; i < mainVars->levelsTextCoords->size; i++)
                    {
                        free(arrayGet(mainVars->levelsTextCoords, i));
                    }
                    if (mainVars->levelsTextCoords->size > 0)
                    {
                        if (!arrayFree(mainVars->levelsTextCoords, false))
                        {
                            fprintf(stderr, "Unable to free array");
                        }
                    }
                    break;
                }
            }

            // prev button
            if (mainVars->levelSelectPrevHover)
            {
                if (mainVars->levelSelectOffset > 0)
                    // remove 1 from offset
                    mainVars->levelSelectOffset--;

                // check if offset is 0
                if (mainVars->levelSelectOffset == 0)
                {
                    // hide button
                    mainVars->levelSelectUpButton = false;
                    // stop hover
                    mainVars->levelSelectPrevHover = false;
                }

                // if next button is not active
                if (!mainVars->levelSelectDownButton)
                {
                    // activate next button
                    mainVars->levelSelectDownButton = true;
                }

                // clear texts from array
                for (int i = 0; i < mainVars->levelsTextCoords->size; i++)
                {
                    free(arrayGet(mainVars->levelsTextCoords, i));
                }
                if (mainVars->levelsTextCoords->size > 0)
                {
                    if (!arrayFree(mainVars->levelsTextCoords, false))
                    {
                        fprintf(stderr, "Unable to free array");
                    }
                }
            }

            // next button
            if (mainVars->levelSelectNextHover)
            {
                if (mainVars->levelSelectOffset < windowProperties->levels->size / LEVELS_PER_PAGE)
                    // add 1 to offset
                    mainVars->levelSelectOffset++;

                // check if current page have less or equal to LEVELS_PER_PAGE
                if (windowProperties->levels->size - mainVars->levelSelectOffset * LEVELS_PER_PAGE <= LEVELS_PER_PAGE)
                {
                    // hide button
                    mainVars->levelSelectDownButton = false;
                    // stop hover
                    mainVars->levelSelectNextHover = false;
                }

                // if prev button is not active
                if (!mainVars->levelSelectUpButton)
                {
                    // activate prev button
                    mainVars->levelSelectUpButton = true;
                }

                // clear texts from array
                for (int i = 0; i < mainVars->levelsTextCoords->size; i++)
                {
                    free(arrayGet(mainVars->levelsTextCoords, i));
                }
                if (mainVars->levelsTextCoords->size > 0)
                {
                    if (!arrayFree(mainVars->levelsTextCoords, false))
                    {
                        fprintf(stderr, "Unable to free array");
                    }
                }
            }

            // back button
            if (mainVars->levelSelectBackHover)
            {
                windowProperties->currentMenu = MainMenu;
                mainVars->levelSelectBackHover = false;
                // reset offset
                mainVars->levelSelectOffset = 0;

                // clear texts from array
                for (int i = 0; i < mainVars->levelsTextCoords->size; i++)
                {
                    free(arrayGet(mainVars->levelsTextCoords, i));
                }
                if (mainVars->levelsTextCoords->size > 0)
                {
                    if (!arrayFree(mainVars->levelsTextCoords, false))
                    {
                        fprintf(stderr, "Unable to free array");
                    }
                }
            }
        }
        else if (windowProperties->currentMenu == LevelInfo)
        {
            // back button
            if (mainVars->levelInfoBackHover)
            {
                windowProperties->currentMenu = LevelSelect;
                mainVars->levelInfoBackHover = false;
            }

            // play button
            if (mainVars->levelInfoStartHover)
            {
                windowProperties->currentMenu = Game;
                mainVars->levelInfoStartHover = false;

                // set defalt variables by level
                windowProperties->score = 0;
                windowProperties->lives = windowProperties->currentLevel->health;
                windowProperties->paddleSpeed = 0;
                windowProperties->paddleX = ((WINDOW_WIDTH * scale) - (windowProperties->textures->paddle->width * scale * 3)) / 2;
                // 80 pixels from botton
                windowProperties->paddleY = WINDOW_HEIGHT * scale - 80;
                // ball
                windowProperties->ballX = ((WINDOW_WIDTH * scale) - (windowProperties->textures->ball->width * scale * 0.66)) / 2;
                windowProperties->ballY = windowProperties->paddleY - (windowProperties->textures->ball->height * scale * 0.66);

                // default ball direction is top-right
                windowProperties->ballSpeedX = 1;
                windowProperties->ballSpeedY = -1;
                windowProperties->ballSpeedModifier = windowProperties->currentLevel->ballSpeed;
                // if username is not allocated, allocate it XD
                if (!windowProperties->currentUserName)
                {
                    windowProperties->currentUserName = (char *)malloc(sizeof(char) * 20);
                }
                windowProperties->currentUserName[0] = '\0';
            }
        }
        else if (windowProperties->currentMenu == GameOver)
        {
            if (mainVars->gameOverSaveHover)
            {
                int len = count_utf8_code_points(windowProperties->currentUserName);
                if (len > 0)
                {
                    // initilize highscores if not
                    Highscores *highscores = windowProperties->highscores;
                    if (!highscores)
                    {
                        initializeHighscores(highscores, windowProperties, mainVars);
                        highscores = windowProperties->highscores;
                    }

                    int score = windowProperties->score;
                    char *scoreText = (char *)malloc(sizeof(char) * 20);
                    sprintf(scoreText, "%d", score);

                    printf("%s - %s\n", windowProperties->currentUserName, scoreText);

                    addHighscore(windowProperties->highscores, windowProperties->currentUserName, scoreText);

                    free(scoreText);

                    sortHighscores(windowProperties->highscores);
                    writeHighscores(windowProperties->highscores);
                    windowProperties->currentMenu = LevelSelect;
                    mainVars->gameOverSaveHover = false;
                }
            }

            if (mainVars->gameOverBackHover)
            {
                windowProperties->currentMenu = LevelSelect;
                mainVars->gameOverBackHover = false;
            }
        }
        else if (windowProperties->currentMenu == Controls)
        {
            // back text
            if (mainVars->controlsBackHover)
            {
                windowProperties->currentMenu = MainMenu;
                mainVars->controlsBackHover = false;
            }
        }
    }

    if (windowProperties->currentMenu == Game)
    {
        if (e->type == SDL_KEYDOWN)
        {
            if (e->key.keysym.sym == SDLK_LEFT || e->key.keysym.sym == SDLK_a)
            {

                windowProperties->paddleSpeed = PADDLE_SPEED;

                if (windowProperties->paddleX > 0)
                {
                    windowProperties->paddleX -= windowProperties->paddleSpeed;
                }
                if (windowProperties->paddleX < 0)
                {
                    windowProperties->paddleX = 0;
                }
            }
            else if (e->key.keysym.sym == SDLK_RIGHT || e->key.keysym.sym == SDLK_d)
            {

                windowProperties->paddleSpeed = PADDLE_SPEED;

                if (windowProperties->paddleX < WINDOW_WIDTH * scale - (windowProperties->textures->paddle->width * scale * 3))
                {
                    windowProperties->paddleX += windowProperties->paddleSpeed;
                }
                if (windowProperties->paddleX > WINDOW_WIDTH * scale - (windowProperties->textures->paddle->width * scale * 3))
                {
                    windowProperties->paddleX = WINDOW_WIDTH * scale - (windowProperties->textures->paddle->width * scale * 3);
                }
            }
            else if (e->key.keysym.sym == SDLK_ESCAPE)
            {
                windowProperties->currentMenu = LevelSelect;
            }
        }
        else if (e->type == SDL_KEYUP)
        {
            if (e->key.keysym.sym == SDLK_LEFT || e->key.keysym.sym == SDLK_a)
            {
                windowProperties->paddleSpeed = 0;
            }
            else if (e->key.keysym.sym == SDLK_RIGHT || e->key.keysym.sym == SDLK_d)
            {
                windowProperties->paddleSpeed = 0;
            }
        }
    }
    else if (windowProperties->currentMenu == GameOver)
    {
        if (e->type == SDL_KEYDOWN)
        {
            int len = count_utf8_code_points(windowProperties->currentUserName);

            if (e->key.keysym.sym >= SDLK_a && e->key.keysym.sym <= SDLK_z)
            {
                if (len < 20)
                {
                    char c = (e->key.keysym.sym - 'a') + 'A';
                    char temp[20];
                    strcpy(temp, windowProperties->currentUserName);
                    snprintf(windowProperties->currentUserName, 20, "%s%c", temp, c);
                }
            }
            else if (e->key.keysym.sym >= SDLK_0 && e->key.keysym.sym <= SDLK_9)
            {
                if (len < 20)
                {
                    char c = e->key.keysym.sym;
                    char temp[20];
                    strcpy(temp, windowProperties->currentUserName);
                    snprintf(windowProperties->currentUserName, 20, "%s%c", temp, c);
                }
            }
            else if (e->key.keysym.sym >= SDLK_KP_1 && e->key.keysym.sym <= SDLK_KP_0)
            {
                if (len < 20)
                {
                    // for some reason SDLK_KP_0 (10737419[22]) is after SDLK_KP_9 (10737419[21])
                    char c = e->key.keysym.sym > SDLK_KP_9 ? '0' : e->key.keysym.sym - SDLK_KP_1 + '1';
                    char temp[20];
                    strcpy(temp, windowProperties->currentUserName);
                    snprintf(windowProperties->currentUserName, 20, "%s%c", temp, c);
                }
            }
            else if (e->key.keysym.sym == SDLK_BACKSPACE && len > 0)
            {
                windowProperties->currentUserName[len - 1] = '\0';
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