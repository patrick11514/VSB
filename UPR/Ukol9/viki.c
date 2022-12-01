#include <stdio.h>

int binSearch(int n, int *arr, int start, int end)
{
    int result = -1;
    int midIndex = (start + end) / 2;

    if (arr[midIndex] == n)
    {
        result = midIndex;
    }

    if (arr[midIndex] > n)
    {
        end = midIndex - 1;
    }
    else
    {
        start = midIndex + 1;
    }

    if (result >= 0)
    {
        return result;
    }
    else
    {
        if (start > end)
        {
            return -1;
        }
        return binSearch(n, arr, start, end);
    }
}

int main()
{
    printf("Zadej cislo: ");
    int n;
    scanf("%d", &n);
    int arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    int start = 0;
    int end = sizeof(arr) / sizeof(int) - 1;

    printf("Cislo je na indexu: %d \n", binSearch(n, arr, start, end));
    return 0;
}