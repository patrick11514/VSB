#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

void nactiSouradnice(int *x, int *y)
{
    int a, b;
    printf("Zadejte souradnice: ");
    scanf("%d %d", &a, &b);

    printf("Nacteno (%d,%d)\n", a, b);

    *x = a;
    *y = b;
}

int main()
{
    int x = 0, y = 0;
    int pole[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    scanf("%d", pole);

    printf("%d %d %d", pole[0], pole[1], pole[2]);

    return 0;
}
