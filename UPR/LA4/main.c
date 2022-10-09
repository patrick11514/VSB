#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    int x = 0, y = 0;
    int pole[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    for (int i = 0; i < 10; i++)
    {
        scanf("%d", pole + i);
    }

    for (int i = 0; i < 10; i++)
    {
        printf("%d\n", pole[i]);
    }

    return 0;
}
