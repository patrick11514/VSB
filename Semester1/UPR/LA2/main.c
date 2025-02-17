#include <stdio.h>

int main()
{
    /*int x;

    printf("Zadejte číslo: ");
    scanf("%d", &x);

    if (x < 100)
    {
        printf("Číslo je malé\n");
    }
    else if (x > 1000)
    {
        printf("Číslo je velké\n");
    }
    else
    {
        printf("Číslo je středí\n");
    }*/

    printf("1 - Max\n");
    printf("2 - Suda\n");
    printf("3 - Součty\n");
    printf("4 - Fibonaci\n");
    int ch;
    printf("Zadej hodnotu: ");
    scanf("%d", &ch);

    switch (ch)
    {
    case 1:
    {
        int max;
        printf("Zadej max číslo: ");
        scanf("%d", &max);

        for (int i = 0; i <= max; i++)
        {
            printf("%d ", i);
        }

        break;
    }
    case 2:
    {
        int max;
        printf("Zadej max číslo: ");
        scanf("%d", &max);

        for (int i = 0; i <= max; i += 2)
        {
            printf("%d ", i);
        }
        break;
    }
    case 3:
    {
        int max;
        printf("Zadej max číslo: ");
        scanf("%d", &max);
        int soucet = 0;

        for (int i = 0; soucet <= max; i += 2)
        {
            soucet += i;
            printf("%d ", soucet);
        }
        break;
    }
    case 4:
    {
        break;
    }
    default:
    {
        printf("Neplatná volba.");
        break;
    }
    }
    printf("\n");

    return 0;
}