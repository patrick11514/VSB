// h files
#include "vector.h"
// libraries
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

Vector *vectorInit(int capacity)
{
    Vector *vector = (Vector *)malloc(sizeof(Vector));
    if (!vector)
    {
        return NULL;
    }

    vector->capacity = capacity;
    vector->size = 0;
    vector->data = (void **)malloc(sizeof(void *) * capacity);

    return vector;
}

bool vectorAdd(Vector *vector, void *item)
{
    if (vector->size == vector->capacity)
    {
        if (vector->capacity == 0)
        {
            vector->capacity = _VECTOR_DEFAULT_CAPACITY;
        }
        else
        {
            vector->capacity *= 2;
        }
        vector->data = (void **)realloc(vector->data, sizeof(void **) * vector->capacity);
        if (!vector->data)
        {
            return false;
        }
    }

    vector->data[vector->size] = item;
    vector->size++;

    return true;
}

void *vectorGet(Vector *vector, int index)
{
    if (index > vector->size || index < 0)
    {
        return NULL;
    }

    return vector->data[index];
}

bool vectorFree(Vector *vector, bool vectorToo)
{
    if ((vector->size == 0 || vector->capacity == 0) && !vectorToo)
    {
        return false;
    }

    free(vector->data);
    if (!vectorToo)
    {
        vector->data = NULL;
        vector->size = 0;
        vector->capacity = 0;
    }
    else
    {
        free(vector);
        vector = NULL;
    }
    return true;
}

void *vectorRemove(Vector *vector, int index)
{
    if (index > vector->size || index < 0)
    {
        return NULL;
    }

    void *element = vector->data[index];

    // shift data
    for (int i = index; i < vector->size - 1; i++)
    {
        vector->data[i] = vector->data[i + 1];
    }

    return element;
}