#include <stdlib.h>

typedef struct {
    void **data;
    int capacity;
    int count;
} Vector;

void vectorInit(Vector * self, int capacity);
void vectorPush(Vector* self, void* element);
void*vectorGet(Vector* self, int index);
void vectorFree(Vector* self);