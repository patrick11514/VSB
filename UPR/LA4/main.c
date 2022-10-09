#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

int sectiPole(int *pole)
{
    int soucet = 0;

    for (int i = 0; i < 10; i++)
    {
        soucet += pole[i];
    }

    return soucet;
}

int main()
{
    int x = 0, y = 0;
    int pole[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    printf("Soucet pole je: %d\n", sectiPole(pole));

    return 0;
}
