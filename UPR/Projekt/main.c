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

// SDL2
#include <SDL2/SDL.h>

// ======================= [ MAIN FUNCTION ] ==========================

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
        highscores->players = NULL;
        highscores->scores = NULL;
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
    char *scoreStr = splitInput(NULL);

    if (!scoreStr)
    {
        printf("No score found.");
        exit(1);
    }

    int score = atoi(scoreStr);

    // add new highscore
    addHighscore(highscores, username, score);

    // sort highscores
    sortHighscores(highscores);

    // write highscores to file
    writeHighscores(highscores);
}

int main(int argc, char **argv)
{
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
            }
        }
    }

    if (SDL_Init(SDL_INIT_VIDEO))
    {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Window *window = SDL_CreateWindow("Breakout", 100, 100, 800, 600, SDL_WINDOW_SHOWN);
    if (!window)
    {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer)
    {
        SDL_DestroyWindow(window);
        fprintf(stderr, "SDL_CreateRenderer Error: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Event e;
    bool quit = false;

    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

// =====================================================================
