#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int strlen2(char *arr)
{
    int len = 0;
    while (arr[len] != '\0')
    {
        len++;
    }

    return len;
}

int main()
{
    char *str = "Ahoj";
    int len = strlen2(str);
    printf("Delka je: %d\n", len);

    return 0;
}
