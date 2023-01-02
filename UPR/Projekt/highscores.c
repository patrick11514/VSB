// Basic includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// my includes
#include "highscores.h"
#include "dynamicarray.h"
#include "global.h"

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

        char *newName = (char *)malloc(strlen(name) + 1);
        if (!newName)
        {
            printf("Unable to allocate memory for name");
            exit(1);
        }

        char *score = splitInput(NULL);

        if (!score)
        {
            printf("Unable to read score");
            exit(1);
        }

        char *newScore = (char *)malloc(strlen(score) + 1);

        if (!arrayAdd(highscores->players, strcpy(newName, name)))
        {
            printf("Unable to add player to array");
            exit(1);
        }
        if (!arrayAdd(highscores->scores, strcpy(newScore, score)))
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
    char *nameCopied = (char *)malloc(strlen(name) + 1);

    if (!nameCopied)
    {
        printf("Unable to allocate memory for name");
        exit(1);
    }

    char *scoreCopied = (char *)malloc(strlen(score) + 1);

    if (!scoreCopied)
    {
        printf("Unable to allocate memory for score");
        exit(1);
    }

    if (!arrayAdd(highscores->players, strcpy(nameCopied, name)))
    {
        printf("Unable to add player to array");
        exit(1);
    }
    if (!arrayAdd(highscores->scores, strcpy(scoreCopied, score)))
    {
        printf("Unable to add score to array");
        exit(1);
    }
    highscores->count++;
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
                swap(arrayGetPTR(scores, j), arrayGetPTR(scores, j + 1));
                swap(arrayGetPTR(names, j), arrayGetPTR(names, j + 1));
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