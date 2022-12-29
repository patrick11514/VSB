// basic includes
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

// sdl
#include <SDL2/SDL.h>

// my includes
#include "files.h"
#include "breakout.h"
#include "dynamicarray.h"
#include "assets.h"

FILE *openFile(char *filename, char *mode)
{
    FILE *file = fopen(filename, mode);
    if (!file)
    {
        printf("Nepovedlo se otevřít soubor. (%d - %s)\n", errno, strerror(errno));
    }
    return file;
}

Level *loadLevel(SDL_Renderer *renderer, WindowProperties *windowProperties, char *_fileName)
{
    FILE *file = openFile(_fileName, "rt");
    if (!file)
    {
        return NULL;
    }

    char row[255];

    bool settings = false;
    bool level = false;

    Level *levelData = (Level *)malloc(sizeof(Level));
    levelData->health = -1;
    levelData->description = NULL;
    levelData->bricks = arrayInit(ARRAY_DEFAULT_CAPACITY);
    if (!levelData->bricks)
    {
        printf("Unable to allocate memory for bricks Array.\n");
        exit(1);
    }

    int numbers[6];
    int numbersCount = 0;

    while (fgets(row, 255, file) != NULL)
    {
        // remove new line at the end
        strcpy(row, strtok(row, "\n"));

        // if settings
        if (settings)
        {
            if (strncmp(row, "Lives: ", 7) == 0)
            {
                levelData->health = atoi(row + 7);
            }
            else if (strncmp(row, "Description: ", 13) == 0)
            {
                levelData->description = (char *)malloc(sizeof(char) * (strlen(row) - 13) + 1);
                strcpy(levelData->description, row + 13);
            }
            else if (strstr(row, " Brick: ") != NULL)
            {
                // get first part Name Brick
                char *brickName = strtok(row, ":");
                // get number
                char *livesCh = strtok(NULL, " ");
                if (!livesCh)
                {
                    printf("Unable to get value of lives for brick.\n");
                    exit(1);
                }

                int lives = atoi(livesCh);

                for (int i = 0; i < numbersCount; i++)
                {
                    if (numbers[i] == lives)
                    {
                        printf("Lives of brick is already used.\n");
                        exit(1);
                    }
                }

                numbers[numbersCount] = lives;
                numbersCount++;

                Brick *brick = (Brick *)malloc(sizeof(Brick));
                brick->lives = lives;
                brick->destroyed = false;

                // get filename
                char *color = strtok(brickName, " ");
                strToLower(color);

                if (strcmp(color, "yellow") == 0)
                {
                    brick->texture = windowProperties->textures->brickYellow;
                }
                else if (strcmp(color, "lime") == 0)
                {
                    brick->texture = windowProperties->textures->brickLime;
                }
                else if (strcmp(color, "gray") == 0)
                {
                    brick->texture = windowProperties->textures->brickGray;
                }
                else if (strcmp(color, "red") == 0)
                {
                    brick->texture = windowProperties->textures->brickRed;
                }
                else if (strcmp(color, "purple") == 0)
                {
                    brick->texture = windowProperties->textures->brickPurple;
                }
                else if (strcmp(color, "blue") == 0)
                {
                    brick->texture = windowProperties->textures->brickBlue;
                }
                else
                {
                    printf("Unable to find texture for brick.\n");
                    exit(1);
                }

                if (!arrayAdd(levelData->bricks, brick))
                {
                    printf("Unable to add brick to array.\n");
                    exit(1);
                }
            }
        }

        // check if settings starts
        if (strcmp(row, "%Settings") == 0)
        {
            settings = true;
            continue;
        }
    }

    if (levelData->health == -1)
    {
        printf("Level does not contains number of hearths.\n");
        exit(1);
    }

    if (levelData->description == NULL)
    {
        printf("Level does not contains description.\n");
        exit(1);
    }

    fclose(file);

    printf("%d\n", levelData->health);
    printf("%s\n", levelData->description);

    arrayFree(levelData->bricks, true);
    free(levelData->description);
    free(levelData);

    return levelData;
}