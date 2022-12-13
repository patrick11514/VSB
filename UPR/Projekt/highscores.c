// Basic includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// header files
#include "highscores.h"
#include "vector.h"

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
    highscores->scores = vectorInit(_VECTOR_DEFAULT_CAPACITY);
    highscores->players = vectorInit(_VECTOR_DEFAULT_CAPACITY);
    highscores->count = 0;

    while (fgets(row, 100, file) != NULL)
    {
        char *name = splitInput(row);

        char *scoreStr = splitInput(NULL);
        if (!scoreStr)
            continue;

        char *saveName = (char *)malloc(sizeof(char) * strlen(name) + 1);
        if (!strcpy(saveName, name))
        {
            printf("Unable to copy string");
            exit(1);
        }
        int score = atoi(scoreStr);
    }

    // if number of real scores are lower than lines (Some of lines are corrupted - missing ; etc..)
    if (count != lines)
    {
        // initialize new variables with correct count
        int *realScores = (int *)malloc(sizeof(int) * count);
        char **realPlayers = (char **)malloc(sizeof(char *) * count);

        // copy values from old variables to new
        for (int i = 0; i < count; i++)
        {
            realScores[i] = scores[i];
            realPlayers[i] = players[i];
        }

        // free old pointers
        free(players);
        free(scores);

        // save to highscores object
        highscores->players = realPlayers;
        highscores->scores = realScores;
    }
    else
    {
        // save to highscores object
        highscores->players = players;
        highscores->scores = scores;
    }

    // save count to highscores object
    highscores->count = count;

    return highscores;
}

void addHighscore(Highscores *highscores, char *name, int score)
{
    // allocate memory for current count + 1
    char **players = (char **)malloc(sizeof(char *) * (highscores->count + 1));
    int *scores = (int *)malloc(sizeof(int) * (highscores->count + 1));

    // copy values
    for (int i = 0; i < highscores->count; i++)
    {
        players[i] = highscores->players[i];
        scores[i] = highscores->scores[i];
    }

    // allocate memory for our name
    char *newName = (char *)malloc(sizeof(char) * strlen(name) + 1);
    if (!strcpy(newName, name))
    {
        printf("Unable to copy string");
        exit(1);
    }

    // save it to last place of arrays
    players[highscores->count] = newName;
    scores[highscores->count] = score;

    // free old arrays
    free(highscores->players);
    free(highscores->scores);

    // save new arrays to highscores object
    highscores->players = players;
    highscores->scores = scores;
    highscores->count = highscores->count + 1;
}

void swapScores(int *x, int *y)
{
    int temp = *x;
    *x = *y;
    *y = temp;
}

void swapNames(char **x, char **y)
{
    char *temp = *x;
    *x = *y;
    *y = temp;
}

void sortHighscores(Highscores *highscores)
{
    int *scores = highscores->scores;
    char **names = highscores->players;
    int count = highscores->count;

    // bubble sort
    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - i - 1; j++)
        {
            if (scores[j] < scores[j + 1])
            {
                swapScores(scores + j, scores + j + 1);
                swapNames(names + j, names + j + 1);
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
        fprintf(file, "%s;%d\n", highscores->players[i], highscores->scores[i]);
    }

    // close file
    fclose(file);

    // free char pointers
    for (int i = 0; i < highscores->count; i++)
    {
        free(highscores->players[i]);
    }

    // free char**
    free(highscores->players);
    // free int*
    free(highscores->scores);
    // free Highscores*
    free(highscores);
}

// =====================================================================