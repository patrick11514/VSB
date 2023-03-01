#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void first()
{
    int one, two;
    printf("Zadej 1. číslo: ");
    if (!scanf("%d", &one))
    {
        printf("Zadal jsi neplatnou hodnotu\n");
        return;
    }

    printf("Zadej 2. číslo: ");
    if (!scanf("%d", &two))
    {
        printf("Zadal jsi neplatnou hodnotu\n");
        return;
    }

    if (one > two)
    {
        printf("První číslo musí být menší\n");
        return;
    }

    if (one < 0 || two < 0)
    {
        printf("Musíš zadat kladná čísla\n");
        return;
    }

    printf("Mocniny:\n");

    for (int i = one; i <= two; i++)
    {
        printf("%d\n", i * i);
    }
}

int factorial(int number)
{
    if (number == 1)
        return 1;
    return number * factorial(number - 1);
}

void second()
{
    unsigned long number;
    printf("Zadej číslo: ");
    if (!scanf("%ld", &number))
    {
        printf("Zadal jsi neplatnou hodnotu\n");
        return;
    }

    if (number < 1)
    {
        printf("Zadej číslo větší než 0");
        return;
    }

    printf("Faktoriál: %d\n", factorial(number));
}

void third()
{
    char string[21];

    printf("Zadej text o maximální délce 20 znaků: ");
    if (fgets(string, 21, stdin) == NULL)
    {
        printf("Nepovedlo se načíst vstup");
    }

    int count = 0;

    for (int i = 0; string[i] != '\0'; i++)
    {
        char ch = string[i];
        if (ch >= 'A' && ch <= 'Z')
        {
            count++;
            string[i] = ch - 'A' + 'a';
        }
    }

    printf("Počet velkých písmen: %d\n", count);
    printf("Upravený řetězec: %s\n", string);
}

#define COUNT 8

void fourth()
{
    int n;
    printf("Zadej počet čísel: ");
    if (!scanf("%d", &n))
    {
        printf("Nezadal jsi číslo.\n");
        return;
    }

    if (n < 1)
    {
        printf("Zadej větší číslo, než 0\n");
        return;
    }

    double *arr = (double *)malloc(sizeof(double) * n);
    if (arr == NULL)
    {
        printf("Nepovedlo se alokovat paměť\n");
        return;
    }

    double number;

    for (int i = 0; i < n; i++)
    {
        if (!scanf("%lf", &number))
        {
            printf("Nezadal jsi číslo\n");
            return;
        }

        arr[i] = number;
    }

    double min = __INT_MAX__;
    double max = 0;
    double middle = 0;

    for (int i = 0; i < n; i++)
    {
        double num = arr[i];
        if (num > max)
        {
            max = num;
        }

        if (num < min)
        {
            min = num;
        }

        middle += num;
    }

    printf("Nejmenší číslo: %lf, Největší číslo: %lf\n", min, max);
    printf("Střední hodnota: %lf\n", middle / n);
}

int main()
{
    // first();
    // second();
    // third();
    fourth();

    return 0;
}