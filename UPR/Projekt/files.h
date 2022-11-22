#ifndef FILES_H
#define FILES_H 1

#include <stdio.h>

// Open file called fileName with mode mode
FILE *openFile(char *fileName, char *mode);

// Get number of lines in file
int getLines(FILE *file);

#endif