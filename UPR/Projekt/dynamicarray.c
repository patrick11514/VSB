// h files
#include "dynamicarray.h"
// libraries
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

Array *arrayInit(int capacity)
{
    Array *array = (Array *)malloc(sizeof(Array));
    if (!array)
    {
        return NULL;
    }

    array->capacity = capacity;
    array->size = 0;
    array->data = (void **)malloc(sizeof(void *) * capacity);

    return array;
}

bool arrayAdd(Array *array, void *item)
{
    if (array->size == array->capacity)
    {
        if (array->capacity == 0)
        {
            array->capacity = _VECTOR_DEFAULT_CAPACITY;
        }
        else
        {
            array->capacity *= 2;
        }
        array->data = (void **)realloc(array->data, sizeof(void **) * array->capacity);
        if (!array->data)
        {
            return false;
        }
    }

    array->data[array->size] = item;
    array->size++;

    return true;
}

void *arrayGet(Array *array, int index)
{
    if (index > array->size || index < 0)
    {
        return NULL;
    }

    return array->data[index];
}

bool arrayFree(Array *array, bool arrayToo)
{
    if ((array->size == 0 || array->capacity == 0) && !arrayToo)
    {
        return false;
    }

    free(array->data);
    if (!arrayToo)
    {
        array->data = NULL;
        array->size = 0;
        array->capacity = 0;
    }
    else
    {
        free(array);
        array = NULL;
    }
    return true;
}

void *arrayRemove(Array *array, int index)
{
    if (index > array->size || index < 0)
    {
        return NULL;
    }

    void *element = array->data[index];

    // shift data
    for (int i = index; i < array->size - 1; i++)
    {
        array->data[i] = array->data[i + 1];
    }

    return element;
}