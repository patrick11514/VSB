#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

int *nasobeniMatVek(int *mat, int *vek, int x, int y)
{
    int *vysledek = (int *)calloc(x, sizeof(int));
    for (int i = 0; i < x; i++)
    {
        for (int l = 0; l < y; l++)
        {
            vysledek[i] += mat[i * y + l] * vek[l];
        }
    }

    return vysledek;
}

int main()
{
    int mat[3][4] = {{1, 1, 1, 2}, {2, 3, 4, 5}, {2, 1, 1, 2}};
    int vek[4] = {2, 2, 3, 2};

    int *vysledek = nasobeniMatVek((int *)mat, vek, 3, 4);

    for (int i = 0; i < 3; i++)
    {
        printf("%d ", vysledek[i]);
    }

    free(vysledek);
    return 0;
}
