#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include "int_vector.h"
#include "vector.h"

int main()
{
    /*IntVector v;
    intVectorInit(&v, 2);

    int x;
    while (scanf("%d", &x) == 1)
    {
        intVectorPush(&v, x);
    }

    printf("Nacteno: %d\nKapacita: %d\n", v.count, v.capacity);
    for (int i = 0; i < v.count; i++)
    {
        printf("%d | %d\n", i, intVectorGet(&v, i));
    }

    intVectorFree(&v);*/

    Vector v;
    vectorInit(&v, 2);

    int x;
    while (scanf("%d", &x) == 1)
    {
        int *elem = (int *)malloc(sizeof(int));
        *elem = x;
        vectorPush(&v, elem);
    }

    printf("Nacteno: %d\nKapacita: %d\n", v.count, v.capacity);
    for (int i = 0; i < v.count; i++)
    {
        printf("%d | %d\n", i, *((int *)vectorGet(&v, i)));
    }

    vectorFree(&v);

    return 0;
}
