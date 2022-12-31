#ifndef GLOBAL_H
#define GLOBAL_H 1

// default includes
#include <string.h>
#include <sys/cdefs.h>

// Split input at ; and \n and return output of strtok
char *splitInput(char *input) __attribute_warn_unused_result__;

// get default scale of application by current hight of display resolution
float getScale(int displayHeight) __attribute_warn_unused_result__;

#endif