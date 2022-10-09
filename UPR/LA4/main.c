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

    nactiSouradnice(&x, &y);
    printf("Nacteno v main (%d,%d)\n", x, y);

    return 0;
}
