#ifndef HIGHSCORES_H
#define HIGHSCORES_H 1

// basic includes
#include <sys/cdefs.h>

// my includes
#include "files.h"
#include "global.h"
#include "dynamicarray.h"

// include Highscores struct from breakout, because if struct was here, it caused include loop, because we need Highscores in breakout.h
#include "breakout.h"

// read highscores from given file
Highscores *readHighscores(FILE *file) __attribute_warn_unused_result__;

// Add new highscore to existings one
void addHighscore(Highscores *highscores, char *name, char *score);

// sort from highest to lowest
void sortHighscores(Highscores *highscores);

// write highscores to file
void writeHighscores(Highscores *highscores);

// free memory
void freeHighscores(Highscores *highscores);

#endif