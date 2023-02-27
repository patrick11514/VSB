// basic includes
#include <stdbool.h>
#include <stdio.h>

// SDL
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

// my includes
#include "highscores.h"
#include "breakout.h"
#include "vars.h"
#include "assets.h"
#include "dynamicarray.h"

bool loadTextures(WindowProperties *windowProperties, SDL_Renderer *renderer)
{
    windowProperties->textures = (Textures *)malloc(sizeof(Textures));

    if (!windowProperties->textures)
    {
        return false;
    }

    // paddle
    Texture *paddle = (Texture *)malloc(sizeof(Texture));
    loadTexture(renderer, paddle, "assets/images/paddle.png");
    windowProperties->textures->paddle = paddle;

    // button up
    Texture *buttonUp = (Texture *)malloc(sizeof(Texture));
    loadTexture(renderer, buttonUp, "assets/images/button_up.png");
    windowProperties->textures->buttonUp = buttonUp;

    // button up hover
    Texture *buttonUpHover = (Texture *)malloc(sizeof(Texture));
    loadTexture(renderer, buttonUpHover, "assets/images/button_up_hover.png");
    windowProperties->textures->buttonUpHover = buttonUpHover;

    // button down
    Texture *buttonDown = (Texture *)malloc(sizeof(Texture));
    loadTexture(renderer, buttonDown, "assets/images/button_down.png");
    windowProperties->textures->buttonDown = buttonDown;

    // button down hover
    Texture *buttonDownHover = (Texture *)malloc(sizeof(Texture));
    loadTexture(renderer, buttonDownHover, "assets/images/button_down_hover.png");
    windowProperties->textures->buttonDownHover = buttonDownHover;

    // Bricks
    // yellow
    Texture *brickYellow = (Texture *)malloc(sizeof(Texture));
    loadTexture(renderer, brickYellow, "assets/images/brick_yellow.png");
    windowProperties->textures->brickYellow = brickYellow;

    // lime
    Texture *brickLime = (Texture *)malloc(sizeof(Texture));
    loadTexture(renderer, brickLime, "assets/images/brick_lime.png");
    windowProperties->textures->brickLime = brickLime;

    // gray
    Texture *brickGray = (Texture *)malloc(sizeof(Texture));
    loadTexture(renderer, brickGray, "assets/images/brick_gray.png");
    windowProperties->textures->brickGray = brickGray;

    // red
    Texture *brickRed = (Texture *)malloc(sizeof(Texture));
    loadTexture(renderer, brickRed, "assets/images/brick_red.png");
    windowProperties->textures->brickRed = brickRed;

    // purple
    Texture *brickPurple = (Texture *)malloc(sizeof(Texture));
    loadTexture(renderer, brickPurple, "assets/images/brick_purple.png");
    windowProperties->textures->brickPurple = brickPurple;

    // blue
    Texture *brickBlue = (Texture *)malloc(sizeof(Texture));
    loadTexture(renderer, brickBlue, "assets/images/brick_blue.png");
    windowProperties->textures->brickBlue = brickBlue;

    // heart
    Texture *heart = (Texture *)malloc(sizeof(Texture));
    loadTexture(renderer, heart, "assets/images/heart.png");
    windowProperties->textures->heart = heart;

    // ball
    Texture *ball = (Texture *)malloc(sizeof(Texture));
    loadTexture(renderer, ball, "assets/images/ball.png");
    windowProperties->textures->ball = ball;

    return true;
}

void freeTextures(WindowProperties *windowProperties)
{
    // free textures
    SDL_DestroyTexture(windowProperties->textures->paddle->texture);
    SDL_DestroyTexture(windowProperties->textures->buttonUp->texture);
    SDL_DestroyTexture(windowProperties->textures->buttonDown->texture);
    SDL_DestroyTexture(windowProperties->textures->buttonUpHover->texture);
    SDL_DestroyTexture(windowProperties->textures->buttonDownHover->texture);
    SDL_DestroyTexture(windowProperties->textures->heart->texture);
    SDL_DestroyTexture(windowProperties->textures->ball->texture);

    // bricks
    SDL_DestroyTexture(windowProperties->textures->brickYellow->texture);
    SDL_DestroyTexture(windowProperties->textures->brickLime->texture);
    SDL_DestroyTexture(windowProperties->textures->brickGray->texture);
    SDL_DestroyTexture(windowProperties->textures->brickRed->texture);
    SDL_DestroyTexture(windowProperties->textures->brickPurple->texture);

    free(windowProperties->textures->paddle);
    free(windowProperties->textures->buttonUp);
    free(windowProperties->textures->buttonDown);
    free(windowProperties->textures->buttonUpHover);
    free(windowProperties->textures->buttonDownHover);
    free(windowProperties->textures->heart);
    free(windowProperties->textures->ball);

    // bricks
    free(windowProperties->textures->brickYellow);
    free(windowProperties->textures->brickLime);
    free(windowProperties->textures->brickGray);
    free(windowProperties->textures->brickRed);
    free(windowProperties->textures->brickPurple);
    free(windowProperties->textures->brickBlue);

    free(windowProperties->textures);
}

bool loadWindowProperties(WindowProperties *windowProperties, SDL_Renderer *renderer, Colors *colors, float SCALE, SDL_Window *window)
{

    windowProperties->colors = colors;
    windowProperties->scale = SCALE;
    windowProperties->currentMenu = MainMenu;
    windowProperties->highscores = NULL;

    // default fps text
    snprintf(windowProperties->currentFPS, 10, "FPS: ~~");

    // load fonts
    TTF_Init();
    TTF_Font *font = TTF_OpenFont("assets/fonts/Roboto-Bold.ttf", 24);
    if (!font)
    {
        fprintf(stderr, "TTF_OpenFont Error: %s", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }
    windowProperties->font = font;

    // set username to NULL, so we can check if it's set
    windowProperties->currentUserName = NULL;
    // set current level to null, so we can check if it's set
    windowProperties->currentLevel = NULL;

    return true;
}

void freeWindowProperties(WindowProperties *windowProperties)
{
    // free highscores
    if (windowProperties->highscores != NULL)
    {
        freeHighscores(windowProperties->highscores);
    }

    free(windowProperties->colors);
    TTF_CloseFont(windowProperties->font);
    TTF_Quit();

    if (windowProperties->currentUserName != NULL)
    {
        free(windowProperties->currentUserName);
    }

    // if currentLevel is set, free bricks from it
    if (windowProperties->currentLevel != NULL)
    {
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

    free(windowProperties);
}

void loadVars(WindowProperties *windowProperties, MainVariables *vars, SDL_Window *window)
{
    // paddle main menu
    int paddleStartPosition = (WINDOW_WIDTH * windowProperties->scale / 2) - 100;
    Position paddlePosition = {.x = paddleStartPosition, .y = 0};

    vars->window = window;
    vars->paddlePosition = paddlePosition;
    vars->FPS = 0;
    vars->paddleReverse = false;

    // set default false
    // main menu
    vars->mainMenuPlayHover = false;
    vars->mainMenuSettingsHover = false;
    vars->mainMenuHighscoresHover = false;
    vars->mainMenuExitHover = false;
    vars->mainMenuControlsHover = false;
    // settings menu
    vars->settingsScaleHover = false;
    vars->settingsBackHover = false;
    // highscores menu
    vars->highscoresBackHover = false;
    vars->highscoresNextHover = false;
    vars->highscoresBackHover = false;
    // level select menu
    vars->levelSelectPrevHover = false;
    vars->levelSelectNextHover = false;
    vars->levelSelectBackHover = false;
    vars->levelSelectOffset = 0;
    vars->levelsTextCoords = arrayInit(LEVELS_PER_PAGE);
    // level info menu
    vars->levelInfoStartHover = false;
    vars->levelInfoBackHover = false;
    // game over menu
    vars->gameOverSaveHover = false;
    vars->gameOverBackHover = false;
    // controls menu
    vars->controlsBackHover = false;
}

void freeVars(MainVariables *vars)
{
    for (int i = 0; i < vars->levelsTextCoords->size; i++)
    {
        free(arrayGet(vars->levelsTextCoords, i));
    }

    if (!arrayFree(vars->levelsTextCoords, true))
    {
        fprintf(stderr, "Error freeing array\n");
    }
    free(vars);
}

void loadColors(Colors *colors)
{
    colors->black = (SDL_Color){0, 0, 0, 255};
    colors->white = (SDL_Color){255, 255, 255, 255};
    colors->red = (SDL_Color){255, 0, 0, 255};
    colors->green = (SDL_Color){0, 255, 0, 255};
    colors->blue = (SDL_Color){0, 0, 255, 255};
    colors->yellow = (SDL_Color){255, 255, 0, 255};
    colors->orange = (SDL_Color){255, 165, 0, 255};
}

bool initSDL(SDL_Window **window, SDL_Renderer **renderer, float *scale)
{
    // SDL2 INIT
    if (SDL_Init(SDL_INIT_VIDEO))
    {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return false;
    }

    // Calculate scale based on display resolution
    SDL_DisplayMode DisplayMode;
    if (SDL_GetCurrentDisplayMode(0, &DisplayMode) != 0)
    {
        fprintf(stderr, "Unable to get DisplaMode: %s\n", SDL_GetError());
        return false;
    }

    *scale = getScale(DisplayMode.h);

    // create window with scale
    *window = SDL_CreateWindow("Breakout", 100, 100, WINDOW_WIDTH * (*scale), WINDOW_HEIGHT * (*scale), SDL_WINDOW_SHOWN);
    if (!*window)
    {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }

    // disable resizing
    SDL_SetWindowResizable(*window, SDL_FALSE);

    // create renderer
    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!*renderer)
    {
        SDL_DestroyWindow(*window);
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }

    return true;
}

void loadDefaultLevels(WindowProperties *windowProperties)
{
    if (!arrayAdd(windowProperties->levels, loadLevel(windowProperties, "assets/defaultLevels/level1.yml")))
    {
        fprintf(stderr, "Unable to add level to array.\n");
        exit(1);
    }

    if (!arrayAdd(windowProperties->levels, loadLevel(windowProperties, "assets/defaultLevels/level2.yml")))
    {
        fprintf(stderr, "Unable to add level to array.\n");
        exit(1);
    }

    if (!arrayAdd(windowProperties->levels, loadLevel(windowProperties, "assets/defaultLevels/level3.yml")))
    {
        fprintf(stderr, "Unable to add level to array.\n");
        exit(1);
    }
}