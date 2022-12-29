#ifndef ARRAY_H
#define ARRAY_H 1

// default includes
#include <stdbool.h>
#include <sys/cdefs.h>

// struct
typedef struct Array
{
    int size;
    int capacity;
    void **data;
} Array;

#define ARRAY_DEFAULT_CAPACITY 2

// functions

// Initialize array of capacity __capacity
// return allocated array
Array *arrayInit(int __capacity) __attribute_warn_unused_result__;

// Add element __item to array __array
// return True on success
bool arrayAdd(Array *__array, void *__item) __attribute_warn_unused_result__;

// Get element on index __index from array __array
// If __index is out of range return NULL
void *arrayGet(Array *__array, int __index) __attribute_warn_unused_result__;

// Free data of array __array and ptr on it
// if __array_too is True, it frees Array *
// return True of success
bool arrayFree(Array *__array, bool __array_too) __attribute_warn_unused_result__;

// Remove element on index __index from array __array
// and return it
void *arrayRemove(Array *__array, int __index) __attribute_warn_unused_result__;

#endif