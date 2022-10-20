#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    // 10->16
    /*unsigned int cislo;
    printf("Zadej dekadické číslo.\n");
    scanf("%u", &cislo);
    printf("Dekadické číslo %u je hedadecimálně %X.\n", cislo, cislo);*/

    // 10->2
    /*short cislo;
    printf("Zadej dekadické číslo:\n");
    scanf("%u", &cislo);

    int i = sizeof(cislo) * 8;
    while (i--)
    {
        //
        if (cislo & (1 << i))
        {
            printf("1");
        }
        else
        {
            printf("0");
        }
    }*/

    // 10->2 pokus 2
    short cislo;
    printf("Zadej dekadické číslo:\n");
    scanf("%u", &cislo);

    // 0000000100000100

    int i = sizeof(cislo) * 8;
    printf("%d", i);
    while (i--)
    {
        if (cislo >> i & 1)
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
