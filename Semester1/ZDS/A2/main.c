#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    int n;
    scanf("%d", &n);

    int s = n / 100;
    int zb = n % 100;
    int d = zb / 10;
    int j = zb % 10;

    printf("0000 ");

    // stovky
    int i = sizeof(s);
    while (i--)
    {
        if (s >> i & 1)
        {
            printf("1");
        }
        else
        {
            printf("0");
        }
    }

    printf(" ");

    // desitky
    i = sizeof(d);
    while (i--)
    {
        if (d >> i & 1)
        {
            printf("1");
        }
        else
        {
            printf("0");
        }
    }

    printf(" ");

    // jednotky
    i = sizeof(j);
    while (i--)
    {
        if (j >> i & 1)
        {
            printf("1");
        }
        else
        {
            printf("0");
        }
    }

    printf("\n");

    return 0;
}
