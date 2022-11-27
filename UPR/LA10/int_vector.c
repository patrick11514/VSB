#include "int_vector.h"
#include <stdlib.h>

void intVectorInit(IntVector *self, int capacity)
{
    self->data = (int *)malloc(capacity * sizeof(int));
    self->capacity = capacity;
    self->count = 0;
};
void intVectorPush(IntVector *self, int element)
{
    if (self->capacity == self->count)
    {
        self->capacity *= 2;
        self->data = (int *)realloc(self->data, self->capacity * sizeof(int));
    }

    self->data[self->count] = element;
    self->count++;
};
int intVectorGet(IntVector *self, int index)
{
    return self->data[index];
};
void intVectorFree(IntVector *self)
{
    free(self->data);
    self->data = NULL;
    self->capacity = 0;
    self->count = 0;
};