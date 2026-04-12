#ifndef LIB_CONVERT_H
#define LIB_CONVERT_H

#include "types.h"

void itoc(int input, char *output);
void stoc(short input, char *output);
void ltoc(long input, char *output);

void uitoc(unsigned int input, char *output);
void ustoc(unsigned short input, char *output);
void ultoc(unsigned long input, char *output);

void itoh(int input, char *output);
void stoh(short input, char *output);
void ltoh(long input, char *output);

void uitohex(unsigned int input, char *output);
void ustohex(unsigned short input, char *output);
void ultohex(unsigned long input, char *output);

#endif // LIB_CONVERT_H
