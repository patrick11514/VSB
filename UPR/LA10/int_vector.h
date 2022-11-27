#include <stdlib.h>

typedef struct {
    int *data;
    int capacity;
    int count;
} IntVector;

void intVectorInit(IntVector * self, int capacity);
void intVectorPush(IntVector* self, int element);
int intVectorGet(IntVector* self, int index);
void intVectorFree(IntVector* self);