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

void addNewHigh()
{
    // read file
    FILE *file = openFile("highscores.txt", "rt");

    // initialize highscores
    Highscores *highscores;

    // if file doesn't exist, create new highscores
    if (!file)
    {
        highscores = (Highscores *)malloc(sizeof(Highscores));
        highscores->count = 0;
        highscores->players = arrayInit(_VECTOR_DEFAULT_CAPACITY);
        highscores->scores = arrayInit(_VECTOR_DEFAULT_CAPACITY);
    }
    else
    {
        // read highscores from file
        highscores = readHighscores(file);
        fclose(file);
    }

    char row[100];

    printf("Enter new highscore (username;score):\n");

    if (!fgets(row, 100, stdin))
    {
        printf("Can't read input");
        exit(1);
    }

    char *username = splitInput(row);
    char *score = splitInput(NULL);

    // add new highscore
    addHighscore(highscores, username, score);

    // sort highscores
    sortHighscores(highscores);

    // write highscores to file
    writeHighscores(highscores);
}

int main(int argc, char **argv)
{
    // arguments
    if (argc > 1)
    {
        // check if arguments contains --help
        for (int i = 1; i < argc; i++)
        {
            if (strcmp(argv[i], "--help") == 0)
            {
                printf("Usage: %s --[OPTION]=[VALUE]...\n", argv[0]);
                printf("Options:\n");
                printf("help - show this message\n");
                printf("level - load level from given file\n");
                printf("      - Example %s --level=level1.txt\n", argv[0]);

                return 0;
            }
        }
    }

    // SDL2 INIT
    if (SDL_Init(SDL_INIT_VIDEO))
    {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    // Calculate scale based on display resolution
    float SCALE;
    SDL_DisplayMode DisplayMode;
    if (SDL_GetCurrentDisplayMode(0, &DisplayMode) != 0)
    {
        fprintf(stderr, "Unable to get DisplaMode: %s\n", SDL_GetError());
        return 1;
    }

    SCALE = getScale(DisplayMode.h);

    // create window with scale
    SDL_Window *window = SDL_CreateWindow("Breakout", 100, 100, WINDOW_WIDTH * SCALE, WINDOW_HEIGHT * SCALE, SDL_WINDOW_SHOWN);
    if (!window)
    {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // disable resizing
    SDL_SetWindowResizable(window, SDL_FALSE);

    // create renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer)
    {
        SDL_DestroyWindow(window);
        fprintf(stderr, "SDL_CreateRenderer Error: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // some variables
    SDL_Event e;
    bool quit = false;
    int frames = 0;

    // colors
    Colors *colors = (Colors *)malloc(sizeof(Colors));
    colors->black = (SDL_Color){0, 0, 0, 255};
    colors->white = (SDL_Color){255, 255, 255, 255};
    colors->red = (SDL_Color){255, 0, 0, 255};
    colors->green = (SDL_Color){0, 255, 0, 255};
    colors->blue = (SDL_Color){0, 0, 255, 255};
    colors->yellow = (SDL_Color){255, 255, 0, 255};
    colors->orange = (SDL_Color){255, 165, 0, 255};

    // allocate memory for Window Properties
    WindowProperties *windowProperties = (WindowProperties *)malloc(sizeof(WindowProperties));
    windowProperties->colors = colors;
    windowProperties->scale = SCALE;
    windowProperties->currentMenu = MainMenu;

    // default fps text
    snprintf(windowProperties->currentFPS, 10, "FPS: ~~");
    unsigned long prevTime = time(NULL);

    // load fonts
    TTF_Init();
    TTF_Font *font = TTF_OpenFont("assets/fonts/Roboto-Bold.ttf", 24);
    if (!font)
    {
        fprintf(stderr, "TTF_OpenFont Error: %s", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    windowProperties->font = font;

    // load textures
    windowProperties->textures = (Textures *)malloc(sizeof(Textures));

    // paddle
    Texture *paddle = (Texture *)malloc(sizeof(Texture));
    SDL_Texture *paddleTexture = IMG_LoadTexture(renderer, "assets/images/paddle.png");
    paddle->texture = paddleTexture;
    paddle->width = 20;
    paddle->height = 6;
    windowProperties->textures->paddle = paddle;

    // button up
    Texture *buttonUp = (Texture *)malloc(sizeof(Texture));
    SDL_Texture *buttonUpTexture = IMG_LoadTexture(renderer, "assets/images/button_up.png");
    buttonUp->texture = buttonUpTexture;
    buttonUp->width = 30;
    buttonUp->height = 10;
    windowProperties->textures->buttonUp = buttonUp;

    // button down
    Texture *buttonDown = (Texture *)malloc(sizeof(Texture));
    SDL_Texture *buttonDownTexture = IMG_LoadTexture(renderer, "assets/images/button_down.png");
    buttonDown->texture = buttonDownTexture;
    buttonDown->width = 30;
    buttonDown->height = 10;
    windowProperties->textures->buttonDown = buttonDown;

    // paddle main menu
    int paddleStartPosition = (WINDOW_WIDTH * windowProperties->scale / 2) - 100;
    Position paddlePosition = {.x = paddleStartPosition, .y = 0};

    MainVariables *vars = (MainVariables *)malloc(sizeof(MainVariables));
    vars->window = window;
    vars->paddlePosition = paddlePosition;
    vars->FPS = 0;
    vars->paddleReverse = false;
    vars->highscores = NULL;

    // set default false
    // main menu
    vars->mainMenuPlayHover = false;
    vars->mainMenuSettingsHover = false;
    vars->mainMenuHighscoresHover = false;
    vars->mainMenuExitHover = false;
    // settings menu
    vars->settingsScaleHover = false;
    vars->settingsBackHover = false;
    // highscores menu
    vars->highscoresBackHover = false;

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

    // free highscores
    if (vars->highscores != NULL)
    {
        freeHighscores(vars->highscores);
    }

    // free textures
    SDL_DestroyTexture(windowProperties->textures->paddle->texture);
    SDL_DestroyTexture(windowProperties->textures->buttonUp->texture);
    SDL_DestroyTexture(windowProperties->textures->buttonDown->texture);

    free(windowProperties->textures->paddle);
    free(windowProperties->textures);

    free(windowProperties->colors);
    free(windowProperties);

    free(vars);

    TTF_CloseFont(font);
    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
