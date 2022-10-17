#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

void vypisMatice(int *p, int x, int y)
{
    for (int i = 0; i < x; i++)
    {
        for (int j = 0; j < y; j++)
        {
            printf("%d ", p[i * y + j]);
        }
        printf("\n");
    }
}

void vypisVektoru(int *p, int c)
{
    for (int i = 0; i < c; i++)
    {
        printf("%d ", p[i]);
    }
}

int main()
{
    int *obr = (int *)malloc(2000 * 3000 * sizeof(int));

    if (obr == NULL)
    {
        printf("Nepodařilo se alokovat paměť");
    }
    else
    {
        printf("OK");
    }
    return 0;
}
