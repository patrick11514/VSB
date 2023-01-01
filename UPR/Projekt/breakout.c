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
    else if (windowProperties->currentMenu == LevelInfo)
    {
        levelInfo(renderer, windowProperties, mainVars);
    }
    else if (windowProperties->currentMenu == Game)
    {
        renderGame(renderer, windowProperties, mainVars);
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

            snprintf(text, 255, "%s - %s", (char *)arrayGet(highscores->players, i), (char *)arrayGet(highscores->scores, i));

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
    int levelWidth = 375 * scale;
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
        if (mainVars->levelSelectPrevHover)
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
        Level *level = arrayGet(levels, i);

        char *text = level->description;
        int len = count_utf8_code_points(text);

        // 20px per char
        int levelTextX = (WINDOW_WIDTH * scale / 2) - (len * 20 * scale / 2);

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
                fprintf(stderr, "Error adding to array: %s", SDL_GetError());
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
            fprintf(stderr, "Error rendering text: %s", SDL_GetError());
        }

        // 50px per level
        levelTextY += 50 * scale;
    }

    int downButtonY = levelTextY + 10 * scale;

    if (mainVars->levelSelectDownButton)
    {
        int downButtonX = (WINDOW_WIDTH * scale / 2) - (windowProperties->textures->buttonDown->width * scale / 2);

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

void levelInfo(SDL_Renderer *renderer, WindowProperties *windowProperties, MainVariables *mainVars)
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
    int levelWidth = 350 * scale;
    int levelHeight = 85 * scale;

    int levelX = (WINDOW_WIDTH * scale / 2) - (levelWidth / 2);
    int levelY = titleCoords.y + titleCoords.height + 10 * scale;

    if (!renderText(renderer, "LEVEL INFO", windowProperties->font, windowProperties->colors->white, levelWidth, levelHeight, levelX, levelY))
    {
        fprintf(stderr, "Error rendering text: %s", SDL_GetError());
    }

    // line under level select
    if (!renderRect(renderer, levelX, levelY + levelHeight, levelWidth, 3, windowProperties->colors->white))
    {
        fprintf(stderr, "Error rendering rect: %s", SDL_GetError());
    }

    // level name
    // 20px per char
    int levelLen = count_utf8_code_points(level->description);

    int levelNameWidth = levelLen * 20 * scale;
    int levelNameHeight = 50 * scale;

    int levelNameX = (WINDOW_WIDTH * scale / 2) - (levelNameWidth / 2);
    int levelNameY = levelY + levelHeight + 10 * scale;

    if (!renderText(renderer, level->description, windowProperties->font, windowProperties->colors->white, levelNameWidth, levelNameHeight, levelNameX, levelNameY))
    {
        fprintf(stderr, "Error rendering text: %s", SDL_GetError());
    }

    // health
    char health[20];
    snprintf(health, 20, "HEALTH: %d", level->health);
    int healthLen = count_utf8_code_points(health);

    int healthWidth = healthLen * 20 * scale;
    int healthHeight = 50 * scale;

    // len of text + width of heart + 10px spacing
    int healthX = (WINDOW_WIDTH * scale / 2) - ((healthWidth + windowProperties->textures->heart->width + 10) / 2);
    int healthY = levelNameY + levelNameHeight * scale;

    if (!renderText(renderer, health, windowProperties->font, windowProperties->colors->white, healthWidth, healthHeight, healthX, healthY))
    {
        fprintf(stderr, "Error rendering text: %s", SDL_GetError());
    }

    // heart
    // 10px spacing
    int heartX = healthX + healthWidth + 10;
    int heartY = healthY;

    if (!renderTexture(renderer, windowProperties->textures->heart->texture, heartX, heartY, windowProperties->textures->heart->width * scale, windowProperties->textures->heart->height * scale))
    {
        fprintf(stderr, "Error rendering texture: %s", SDL_GetError());
    }

    int brickY = healthY + healthHeight * scale;

    // available bricks
    for (int i = 0; i < level->brickHealths->size; i++)
    {
        BrickHealth *brickHealth = arrayGet(level->brickHealths, i);

        char text[5];
        snprintf(text, 5, "%d", brickHealth->lives);
        int len = count_utf8_code_points(text);

        // Brick texture Text Heart icon
        int brickHeight = 50 * scale;
        int brickTextureScale = brickHeight / (brickHealth->texture->height * scale);
        //  Brick Width + 10px spacing + text width (20px per char) + 10px spacing + heart width
        int brickWidth = (len * 20 + windowProperties->textures->heart->width + 10 * 2) * scale + brickHealth->texture->width * brickTextureScale;

        int brickX = (WINDOW_WIDTH * scale / 2) - (brickWidth / 2);

        // render brick
        // brickY + 5px for centering
        if (!renderTexture(renderer, brickHealth->texture->texture, brickX, brickY + 5, brickHealth->texture->width * brickTextureScale, brickHealth->texture->height * scale * brickTextureScale))
        {
            fprintf(stderr, "Error rendering texture: %s", SDL_GetError());
        }

        int textX = brickX + brickHealth->texture->width * brickTextureScale + 10 * scale;

        // render text
        if (!renderText(renderer, text, windowProperties->font, windowProperties->colors->white, len * 20 * scale, brickHeight, textX, brickY))
        {
            fprintf(stderr, "Error rendering text: %s", SDL_GetError());
        }

        int heartX = textX + len * 20 * scale + 10 * scale;

        // render heart
        if (!renderTexture(renderer, windowProperties->textures->heart->texture, heartX, brickY, windowProperties->textures->heart->width * scale, windowProperties->textures->heart->height * scale))
        {
            fprintf(stderr, "Error rendering texture: %s", SDL_GetError());
        }

        brickY += brickHeight + 10 * scale;
    }

    // start text
    int startWidth = 175 * scale;
    int startHeight = 85 * scale;

    int startX = (WINDOW_WIDTH * scale / 2) - (startWidth / 2);
    int startY = brickY;

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
        fprintf(stderr, "Error rendering text: %s", SDL_GetError());
    }

    // back text
    int backWidth = 175 * scale;
    int backHeight = 85 * scale;

    int backX = (WINDOW_WIDTH * scale / 2) - (backWidth / 2);
    int backY = startY + startHeight * scale;

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
        fprintf(stderr, "Error rendering text: %s", SDL_GetError());
    }
}

void renderGame(SDL_Renderer *renderer, WindowProperties *windowProperties, MainVariables *mainVars)
{
    float scale = windowProperties->scale;

    // line at top
    int topPadding = 120 * scale;

    if (!renderRect(renderer, 0, topPadding, WINDOW_WIDTH * scale, 3 * scale, windowProperties->colors->white))
    {
        fprintf(stderr, "Error rendering rect: %s", SDL_GetError());
    }

    /**
     * Concept:
     * Numbers will be actually rendered by text and heart will be texture
     * ----------------------------------------
     * |                  |                   |
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
        fprintf(stderr, "Error rendering rect: %s", SDL_GetError());
    }

    // score text
    int scoreTextWidth = 80 * scale;
    int scoreTextHeight = 35 * scale;

    int scoreTextX = (WINDOW_WIDTH * scale / 4) - (scoreTextWidth / 2);
    int scoreTextY = 0;

    if (!renderText(renderer, "Score", windowProperties->font, windowProperties->colors->white, scoreTextWidth, scoreTextHeight, scoreTextX, scoreTextY))
    {
        fprintf(stderr, "Error rendering text: %s", SDL_GetError());
    }

    // score number
    char scoreText[5];
    snprintf(scoreText, 5, "%d", windowProperties->score);
    int scoreLen = count_utf8_code_points(scoreText);

    // 20px per char
    int scoreWidth = scoreLen * 20 * scale;
    int scoreHeight = 50 * scale;

    int scoreX = (WINDOW_WIDTH * scale / 4) - (scoreWidth / 2);
    // 120 is height of space between top and line
    int scoreY = ((120 * scale - scoreTextHeight) / 2) - (scoreHeight / 2) + scoreTextHeight;

    if (!renderText(renderer, scoreText, windowProperties->font, windowProperties->colors->white, scoreWidth, scoreHeight, scoreX, scoreY))
    {
        fprintf(stderr, "Error rendering text: %s", SDL_GetError());
    }

    // lives text
    int livesTextWidth = 80 * scale;
    int livesTextHeight = 35 * scale;

    int livesTextX = (WINDOW_WIDTH * scale / 4) * 3 - (livesTextWidth / 2);
    int livesTextY = 0;

    if (!renderText(renderer, "Lives", windowProperties->font, windowProperties->colors->white, livesTextWidth, livesTextHeight, livesTextX, livesTextY))
    {
        fprintf(stderr, "Error rendering text: %s", SDL_GetError());
    }

    // lives number + heart texture
    char livesText[5];
    snprintf(livesText, 5, "%d", windowProperties->lives);
    int len = count_utf8_code_points(livesText);

    // 20px per char
    int livesWidth = len * 20 * scale;
    int livesHeight = 35 * scale;

    Texture *hearthTexture = windowProperties->textures->heart;

    // scaling for heart texture
    double scaling = (float)livesHeight / hearthTexture->height;

    //+ hearth + spacing 10px
    int livesX = (WINDOW_WIDTH * scale / 4) * 3 - (livesWidth + windowProperties->textures->heart->width * scaling / 2) + 10 * scale;
    int livesY = ((120 * scale - livesTextHeight) / 2) - (livesHeight / 2) + livesTextHeight;

    if (!renderText(renderer, livesText, windowProperties->font, windowProperties->colors->white, livesWidth, livesHeight, livesX, livesY))
    {
        fprintf(stderr, "Error rendering text: %s", SDL_GetError());
    }

    // heart texture
    if (!renderTexture(renderer, hearthTexture->texture, livesX + livesWidth + 10 * scale, livesY, hearthTexture->width * scaling, hearthTexture->height * scaling))
    {
        fprintf(stderr, "Error rendering texture: %s", SDL_GetError());
    }

    //******************** GAME ********************

    // ball with 0.66 scale
    if (!renderTexture(renderer, windowProperties->textures->ball->texture, windowProperties->ballX, windowProperties->ballY, windowProperties->textures->ball->width * scale * 0.66, windowProperties->textures->ball->height * scale * 0.66))
    {
        fprintf(stderr, "Error rendering texture: %s", SDL_GetError());
    }

    // paddle with scale 3
    if (!renderTexture(renderer, windowProperties->textures->paddle->texture, windowProperties->paddleX, windowProperties->paddleY, windowProperties->textures->paddle->width * scale * 3, windowProperties->textures->paddle->height * scale * 3))
    {
        fprintf(stderr, "Error rendering texture: %s", SDL_GetError());
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
                // reset offset
                mainVars->highscoresOffset = 0;
                // reset buttons
                mainVars->highscoresUpButton = false;
                mainVars->highscoresDownButton = true;
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
                    windowProperties->currentLevel = arrayGet(windowProperties->levels, i + mainVars->levelSelectOffset * LEVELS_PER_PAGE);
                    windowProperties->currentMenu = LevelInfo;
                    textCoords->hover = false;
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

                windowProperties->pressingLeft = false;
                windowProperties->pressingRight = false;
            }
        }
    }

    if (windowProperties->currentMenu == Game)
    {
        if (e->type == SDL_KEYDOWN)
        {
            if (e->key.keysym.sym == SDLK_LEFT || e->key.keysym.sym == SDLK_a)
            {
                if (!windowProperties->pressingLeft)
                {
                    windowProperties->pressingLeft = true;
                }

                if (windowProperties->pressingLeft)
                {
                    if (windowProperties->paddleSpeed < PADDLE_MAX_SPEED)
                    {
                        windowProperties->paddleSpeed += PADDLE_SPEED_MODIFIER;
                    }
                    if (windowProperties->paddleX > 0)
                    {
                        windowProperties->paddleX -= windowProperties->paddleSpeed;
                    }
                    if (windowProperties->paddleX < 0)
                    {
                        windowProperties->paddleX = 0;
                    }
                }

                printf("LEFT %f\n", windowProperties->paddleSpeed);
            }
            else if (e->key.keysym.sym == SDLK_RIGHT || e->key.keysym.sym == SDLK_d)
            {
                if (!windowProperties->pressingRight)
                {
                    windowProperties->pressingRight = true;
                }

                if (windowProperties->pressingRight)
                {
                    if (windowProperties->paddleSpeed < PADDLE_MAX_SPEED)
                    {
                        windowProperties->paddleSpeed += PADDLE_SPEED_MODIFIER;
                    }
                    if (windowProperties->paddleX < WINDOW_WIDTH * scale - (windowProperties->textures->paddle->width * scale * 3))
                    {
                        windowProperties->paddleX += windowProperties->paddleSpeed;
                    }
                    if (windowProperties->paddleX > WINDOW_WIDTH * scale - (windowProperties->textures->paddle->width * scale * 3))
                    {
                        windowProperties->paddleX = WINDOW_WIDTH * scale - (windowProperties->textures->paddle->width * scale * 3);
                    }
                }

                printf("RIGHT %f\n", windowProperties->paddleSpeed);
            }
        }
        else if (e->type == SDL_KEYUP)
        {
            if (e->key.keysym.sym == SDLK_LEFT || e->key.keysym.sym == SDLK_a)
            {
                windowProperties->pressingLeft = false;
                windowProperties->paddleSpeed = 0;
                printf("LEFT UP\n");
            }
            else if (e->key.keysym.sym == SDLK_RIGHT || e->key.keysym.sym == SDLK_d)
            {
                windowProperties->pressingRight = false;
                windowProperties->paddleSpeed = 0;
                printf("RIGHT UP\n");
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