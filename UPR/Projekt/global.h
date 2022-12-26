#ifndef GLOBAL_H
#define GLOBAL_H 1

#include <string.h>

// Split input at ; and \n and return output of strtok
char *splitInput(char *input);

// get default scale of application by current hight of display resolution
float getScale(int displayHeight);

#endif