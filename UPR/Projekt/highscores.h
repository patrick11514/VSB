#ifndef HIGHSCORES_H
#define HIGHSCORES_H 1

// my includes
#include "files.h"
#include "global.h"
#include "dynamicarray.h"

// Highscores structure
typedef struct Highscores
{
    int count;
    Array *scores;
    Array *players;
} Highscores;

// read highscores from given file
Highscores *readHighscores(FILE *file);

// Add new highscore to existings one
void addHighscore(Highscores *highscores, char *name, char *score);

// swap names between X and Y
void swap(char **x, char **y);

// sort from highest to lowest
void sortHighscores(Highscores *highscores);

// write highscores to file
void writeHighscores(Highscores *highscores);

// free memory
void freeHighscores(Highscores *highscores);

#endif