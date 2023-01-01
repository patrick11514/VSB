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
        fprintf(stderr, "Unable to open file. (%d - %s)\n", errno, strerror(errno));
    }
    return file;
}

Level *loadLevel(WindowProperties *windowProperties, char *_fileName)
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
    levelData->brickHealths = arrayInit(ARRAY_DEFAULT_CAPACITY);
    if (!levelData->brickHealths)
    {
        fprintf(stderr, "Unable to allocate memory for brick health Array.\n");
        exit(1);
    }
    // array for each line of bricks
    levelData->bricks = arrayInit(ARRAY_DEFAULT_CAPACITY);
    if (!levelData->bricks)
    {
        fprintf(stderr, "Unable to allocate memory for bricks Array.\n");
        exit(1);
    }

    int numbers[6];
    int numbersCount = 0;

    int x = 0, y = 0;

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
            else if (strncmp(row, "Score: ", 7) == 0)
            {
                levelData->scorePerBrick = atoi(row + 7);
            }
            else if (strncmp(row, "Description: ", 13) == 0)
            {
                char *text = row + 13;
                int len = count_utf8_code_points(text);

                if (len > 20)
                {
                    fprintf(stderr, "Description is longer than 20 chars (%d).\n", len);
                    exit(1);
                }

                levelData->description = (char *)malloc(sizeof(char) * (strlen(row) - 13) + 1);
                strcpy(levelData->description, text);
            }
            else if (strstr(row, " Brick: ") != NULL)
            {
                // get first part Name Brick
                char *brickName = strtok(row, ":");
                // get number
                char *livesCh = strtok(NULL, " ");
                if (!livesCh)
                {
                    fprintf(stderr, "Unable to get value of lives for brick.\n");
                    exit(1);
                }

                int lives = atoi(livesCh);

                for (int i = 0; i < numbersCount; i++)
                {
                    if (numbers[i] == lives)
                    {
                        fprintf(stderr, "Lives of brick is already used.\n");
                        exit(1);
                    }
                }

                numbers[numbersCount] = lives;
                numbersCount++;

                BrickHealth *brick = (BrickHealth *)malloc(sizeof(Brick));
                brick->lives = lives;

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
                    fprintf(stderr, "Unable to find texture for brick.\n");
                    exit(1);
                }

                if (!arrayAdd(levelData->brickHealths, brick))
                {
                    fprintf(stderr, "Unable to add brick to array.\n");
                    exit(1);
                }
            }
            else if (strcmp(row, "%EndSettings") == 0)
            {
                settings = false;
                continue;
            }
        }

        // check if settings starts
        if (strcmp(row, "%Settings") == 0)
        {
            settings = true;
            continue;
        }

        // if level
        if (level)
        {
            if (strcmp(row, "%EndLevel") == 0)
            {
                level = false;
                continue;
            }

            int rowLength = strlen(row);

            if (rowLength > 17)
            {
                fprintf(stderr, "Row is longer than 17 chars (%d).\n", rowLength);
                exit(1);
            }

            if (levelData->bricks->size == 24)
            {
                fprintf(stderr, "Level is longer than 24 rows.\n");
                exit(1);
            }

            Array *bricksLine = arrayInit(rowLength);

            for (int i = 0; i < rowLength; i++)
            {
                char brickChar = row[i];

                Brick *brick = (Brick *)malloc(sizeof(Brick));
                brick->destroyed = false;
                brick->x = x;
                brick->y = y;

                if (brickChar == 'Y')
                {
                    brick->texture = windowProperties->textures->brickYellow;
                }
                else if (brickChar == 'L')
                {
                    brick->texture = windowProperties->textures->brickLime;
                }
                else if (brickChar == 'G')
                {
                    brick->texture = windowProperties->textures->brickGray;
                }
                else if (brickChar == 'R')
                {
                    brick->texture = windowProperties->textures->brickRed;
                }
                else if (brickChar == 'P')
                {
                    brick->texture = windowProperties->textures->brickPurple;
                }
                else if (brickChar == 'B')
                {
                    brick->texture = windowProperties->textures->brickBlue;
                }
                else
                {
                    fprintf(stderr, "Unable to find texture for brick.\n");
                    exit(1);
                }

                if (!arrayAdd(bricksLine, brick))
                {
                    fprintf(stderr, "Unable to add brick to array.\n");
                    exit(1);
                }

                x += 1;
            }

            if (!arrayAdd(levelData->bricks, bricksLine))
            {
                fprintf(stderr, "Unable to add brick line to array.\n");
                exit(1);
            }

            y += 1;
        }

        // check if level starts
        if (strcmp(row, "%Level") == 0)
        {
            level = true;
            continue;
        }
    }

    if (levelData->health == -1)
    {
        fprintf(stderr, "Level does not contains number of hearts.\n");
        exit(1);
    }

    if (levelData->description == NULL)
    {
        fprintf(stderr, "Level does not contains description.\n");
        exit(1);
    }

    fclose(file);

    return levelData;
}

void freeLevels(Array *levels)
{
    for (int i = 0; i < levels->size; i++)
    {
        Level *levelData = arrayGet(levels, i);

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

        for (int i = 0; i < levelData->brickHealths->size; i++)
        {
            BrickHealth *brickHealth = (BrickHealth *)arrayGet(levelData->brickHealths, i);
            free(brickHealth);
        }

        if (!arrayFree(levelData->bricks, true))
        {
            fprintf(stderr, "Unable to free bricks array.\n");
        }
        if (!arrayFree(levelData->brickHealths, true))
        {
            fprintf(stderr, "Unable to free brickHealths array.\n");
        }
        free(levelData->description);
        free(levelData);
    }

    if (!arrayFree(levels, true))
    {
        fprintf(stderr, "Unable to free levels array.\n");
    }
}