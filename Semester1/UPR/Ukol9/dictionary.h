#ifndef _DICTIONARY_H
#define _DICTIONARY_H 1

#include "vector.h"
#include <stdbool.h>
#include <stdio.h>

typedef struct
{
    int size;
    Vector *en;
    Vector *cz;
} Dictionary;

// Initialize dictionary
// return true on success
bool dictionaryInit(Dictionary *__dict) __attribute_warn_unused_result__;

// Free memory of dictionary
// return true on success
bool dictionaryFree(Dictionary *__dict) __attribute_warn_unused_result__;

// Add en and cz to __dict
bool dictionaryAdd(Dictionary *__dict, char *en, char *cz) __attribute_warn_unused_result__;

// If __inverse is false, it finds __string in en words and return cz word
// If __inverse is true, it fines __string in cz words and retrun en word
// If word is not found, return NULL
void *dictionaryGet(Dictionary *__dict, char *__string, bool __inverse) __attribute_warn_unused_result__;

// Load dictionary from file __file and save it to __dict
void dictionaryLoad(FILE *__file, Dictionary *__dict);

#endif