#include <stdio.h>

int main()
{
    int n, m, f;
    printf("cislo=");
    scanf("%x", &n);

    printf("m=");
    scanf("%d", &m);
    printf("f=");
    scanf("%d", &f);

    int des = 4 * 3 - f;
    int i = sizeof(n) * 3;
    while (i--)
    {
        if (n >> i & 1)
        {
            printf("1");
        }
        else
        {
            printf("0");
        }

        des--;
        if (des == 0)
        {
            printf(",");
        }
    }

    printf("\n");

    return 0;
}
