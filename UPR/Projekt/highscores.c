// Basic includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// my includes
#include "highscores.h"
#include "dynamicarray.h"

// ======================= [ HIGHSCORE FUNCTIONS ] =====================

Highscores *readHighscores(FILE *file)
{

    char row[100];

    Highscores *highscores = (Highscores *)malloc(sizeof(Highscores));
    if (!highscores)
    {
        printf("Unabel to allocate memory for highscores");
        exit(1);
    }
    highscores->scores = arrayInit(ARRAY_DEFAULT_CAPACITY);
    highscores->players = arrayInit(ARRAY_DEFAULT_CAPACITY);
    highscores->count = 0;

    while (fgets(row, 100, file) != NULL)
    {
        char *name = splitInput(row);

        if (!name)
        {
            printf("Unable to read name");
            exit(1);
        }

        char *score = splitInput(NULL);

        if (!score)
        {
            printf("Unable to read score");
            exit(1);
        }

        if (!arrayAdd(highscores->players, strdup(name)))
        {
            printf("Unable to add player to array");
            exit(1);
        }
        if (!arrayAdd(highscores->scores, strdup(score)))
        {
            printf("Unable to add score to array");
            exit(1);
        }

        highscores->count++;
    }

    return highscores;
}

void addHighscore(Highscores *highscores, char *name, char *score)
{
    char *nameCopied = strdup(name);

    if (!nameCopied)
    {
        printf("Unable to copy name");
        exit(1);
    }

    char *scoreCopied = strdup(score);

    if (!scoreCopied)
    {
        printf("Unable to copy score");
        exit(1);
    }

    if (!arrayAdd(highscores->players, nameCopied))
    {
        printf("Unable to add player to array");
        exit(1);
    }
    if (!arrayAdd(highscores->scores, scoreCopied))
    {
        printf("Unable to add score to array");
        exit(1);
    }
    highscores->count++;
}

void swap(char **x, char **y)
{
    char *temp = *x;
    *x = *y;
    *y = temp;
}

void sortHighscores(Highscores *highscores)
{
    Array *scores = highscores->scores;
    Array *names = highscores->players;
    int count = highscores->count;

    // bubble sort
    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - i - 1; j++)
        {
            if (atoi((char *)arrayGet(scores, j)) < atoi((char *)arrayGet(scores, j + 1)))
            {
                swap((char **)arrayGetPTR(scores, j), (char **)arrayGetPTR(scores, j + 1));
                swap((char **)arrayGetPTR(names, j), (char **)arrayGetPTR(names, j + 1));
            }
        }
    }
}

void writeHighscores(Highscores *highscores)
{
    // open file as write
    FILE *file = openFile("highscores.txt", "wt");

    // save scores to file
    for (int i = 0; i < highscores->count; i++)
    {
        fprintf(file, "%s;%s\n", (char *)arrayGet(highscores->players, i), (char *)arrayGet(highscores->scores, i));
    }

    // close file
    fclose(file);
}

void freeHighscores(Highscores *highscores)
{
    // free data
    for (int i = 0; i < highscores->count; i++)
    {
        free(arrayGet(highscores->players, i));
        free(arrayGet(highscores->scores, i));
    }

    // free arrays
    if (!arrayFree(highscores->players, true))
    {
        printf("Unable to free memory");
        exit(1);
    }
    if (!arrayFree(highscores->scores, true))
    {
        printf("Unable to free memory");
        exit(1);
    }
    free(highscores);
}

// =====================================================================