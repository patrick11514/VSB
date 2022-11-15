// Basic includes
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// my includes
#include "files.c"
#include "global.c"

// =========================== [ TYPES ] ===============================
typedef struct
{
    int count;
    int *scores;
    char **players;
} Highscores;

// =====================================================================

// ======================= [ HIGHSCORE FUNCTIONS ] =====================

// read highscores from given file
Highscores *readHighscores(FILE *file)
{
    char row[100];

    int count = 0;

    int lines = getLines(file);

    // tries to allocate players and scores by number of lines in file

    int *scores = (int *)malloc(sizeof(int) * lines);
    char **players = (char **)malloc(sizeof(char *) * lines);

    if (!scores || !players)
    {
        printf("Unable to allocate memory");
        exit(1);
    }

    fseek(file, 0, 0);

    while (fgets(row, 100, file) != NULL)
    {
        char *name = splitInputAt(row);

        char *scoreStr = splitInputAt(NULL);
        if (!scoreStr)
            continue;

        char *saveName = (char *)malloc(sizeof(char) * strlen(name) + 1);
        if (!strcpy(saveName, name))
        {
            printf("Unable to copy string");
            exit(1);
        }
        int score = atoi(scoreStr);

        players[count] = saveName;
        scores[count] = score;

        count++;
    }

    Highscores *highscores = (Highscores *)malloc(sizeof(Highscores));

    if (!highscores)
    {
        printf("Unable to allocate memory");
        exit(1);
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

// Add new highscore to existings one
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

// swap scores between X and Y
void swapScores(int *x, int *y)
{
    int temp = *x;
    *x = *y;
    *y = temp;
}

// swap names between X and Y
void swapNames(char **x, char **y)
{
    char *temp = *x;
    *x = *y;
    *y = temp;
}

// sort from highest to lowest
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

// write highscores to file
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