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

int main()
{
    int p[4][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {10, 11, 12}};

    vypisMatice((int *)p, 4, 3);
    return 0;
}
