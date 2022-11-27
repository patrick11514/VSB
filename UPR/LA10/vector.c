#include "vector.h"
#include <stdlib.h>

void vectorInit(Vector *self, int capacity)
{
    self->data = (void **)malloc(capacity * sizeof(void *));
    self->capacity = capacity;
    self->count = 0;
};
void vectorPush(Vector *self, void *element)
{
    if (self->capacity == self->count)
    {
        self->capacity *= 2;
        self->data = (void **)realloc(self->data, self->capacity * sizeof(void *));
    }

    self->data[self->count] = element;
    self->count++;
};
void *vectorGet(Vector *self, int index)
{
    return self->data[index];
};
void vectorFree(Vector *self)
{
    for (int i = 0; i < self->count; i++)
    {
        free(self->data[i]);
    }
    free(self->data);
    self->data = NULL;
    self->capacity = 0;
    self->count = 0;
};