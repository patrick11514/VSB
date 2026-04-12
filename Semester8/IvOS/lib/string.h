#ifndef LIB_STRING_H
#define LIB_STRING_H

#include "types.h"

void *memcpy(void *destination, const void *source, size_t size);
void *memset(void *destination, int value, size_t size);
int memcmp(const void *left, const void *right, size_t size);

size_t strlen(const char *str);
int strcmp(const char *left, const char *right);
int strncmp(const char *left, const char *right, size_t size);
char *strchr(const char *str, int value);
char *strncpy(char *destination, const char *source, size_t size);
void strcpy(char *destination, const char *source);

#endif // LIB_STRING_H
