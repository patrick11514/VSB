#ifndef _VECTOR_H
#define _VECTOR_H 1

#include <stdbool.h>
#include <sys/cdefs.h>

// struct
typedef struct
{
    int size;
    int capacity;
    void **data;
} Vector;

#define _VECTOR_DEFAULT_CAPACITY 2

// functions

// Initialize vector of capacity __capacity
// return allocated vector
Vector *vectorInit(int __capacity) __attribute_warn_unused_result__;

// Add element __item to vector __vector
// return True on success
bool vectorAdd(Vector *__vector, void *__item) __attribute_warn_unused_result__;

// Get element on index __index from vector __vector
// If __index is out of range return NULL
void *vectorGet(Vector *__vector, int __index) __attribute_warn_unused_result__;

// Free data of vector __vector and ptr on it
// if __vector_too is True, it frees Vector *
// return True of success
bool vectorFree(Vector *__vector, bool __vector_too) __attribute_warn_unused_result__;

// Remove element on index __index from vector __vector
// and return it
void *vectorRemove(Vector *__vector, int __index) __attribute_warn_unused_result__;

#endif