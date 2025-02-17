#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

void setZeros(int *arr, int size)
{
    for (int i = 0; i < size; i++)
    {
        arr[i] = 0;
    }
}

void getNumbersFromUser(int *arr, int numbersCount)
{
    for (int i = 0; i < numbersCount; i++)
    {
        int number;
        scanf("%d", &number);

        // if number is out of range 1-9, then add to number 1
        if (number > 9 || number < 1)
        {
            arr[0]++;
        }
        else
        {
            arr[number - 1]++;
        }
    }
}

void printHistogram(char mode, int *arr, int size)
{
    if (mode == 'v')
    {
        for (int i = 0; i < size; i++)
        {
            printf("%d ", i + 1);
            for (int j = 0; j < arr[i]; j++)
            {
                printf("#");
            }
            printf("\n");
        }
    }
    else
    {
        int highest = 0;
        for (int i = 0; i < size; i++)
        {
            if (arr[i] > highest)
            {
                highest = arr[i];
            }
        }

        for (int i = highest; i >= 0; i--)
        {
            if (i > 0)
            {
                for (int j = 0; j < size; j++)
                {
                    if (arr[j] >= i)
                    {
                        printf("#");
                    }
                    else
                    {
                        printf(" ");
                    }
                }
                printf("\n");
            }
            else
            {
                printf("123456789\n");
            }
        }
    }
}

int main()
{
    // draw mode
    char mode;
    scanf("%c", &mode);

    if (mode != 'v' && mode != 'h')
    {
        printf("Neplatny mod vykresleni\n");
        return 1;
    }

    // number of numbers
    int n;
    scanf("%d", &n);

    int numbers[9];

    // set zeros to array
    setZeros(numbers, 9);

    // read numbers
    getNumbersFromUser(numbers, n);

    // print histogram
    printHistogram(mode, numbers, 9);

    return 0;
}
