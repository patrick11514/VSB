#ifndef HIGHSCORES_H
#define HIGHSCORES_H 1

// includes
#include "files.h"
#include "global.h"

// sctuctures
typedef struct
{
    int count;
    int *scores;
    char **players;
} Highscores;

// read highscores from given file
Highscores *readHighscores(FILE *file);

// Add new highscore to existings one
void addHighscore(Highscores *highscores, char *name, int score);

// swap scores between X and Y
void swapScores(int *x, int *y);

// swap names between X and Y
void swapNames(char **x, char **y);

// sort from highest to lowest
void sortHighscores(Highscores *highscores);

// write highscores to file
void writeHighscores(Highscores *highscores);

#endif