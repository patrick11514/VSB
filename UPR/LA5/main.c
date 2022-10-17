#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

int *nacti()
{
    int c;
    scanf("%d", &c);
    int *vek = (int *)malloc(c * sizeof(int));
    for (int i = 0; i < c; i++)
    {
        scanf("%d", vek + i);
    }
    return vek;
}

int main()
{
    int *vek = nacti();
    for (int i = 0; i < 10; i++)
    {
        printf("%d ", vek[i]);
    }
    free(vek);
    return 0;
}
