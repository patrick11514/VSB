#include <stdio.h>

void line(int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("x");
    }
    printf("\n");
}

void col(int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("x\n");
    }
}

void square(int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int l = 0; l < n; l++)
            printf("x");
        printf("\n");
    }
}

void rect(int w, int h)
{
    for (int i = 0; i < h; i++)
    {
        for (int l = 0; l < w; l++)
            printf("x");
        printf("\n");
    }
}

void ladder(int n, int w)
{
    for (int i = 0; i < n; i++)
    {
        for (int l = 0; l < w; l++)
        {
            printf("x");
        }

        if (i != n - 1)
            printf("\n\n");
        else
            printf("\n");
    }
}

void square2(int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int l = 0; l < n; l++)
        {
            if (i == 0 || i == n - 1)
            {
                printf("x ");
            }
            else if (l == 0 || l == n - 1)
            {
                printf("x ");
            }
            else
            {
                printf("  ");
            }
        }
        printf("\n");
    }
}

void diagr(int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int l = 0; l < i; l++)
        {
            printf(" ");
        }
        printf("x\n");
    }
}

void diagl(int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int l = 0; l < n - i - 1; l++)
        {
            printf(" ");
        }
        printf("x\n");
    }
}

void triangle(int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int l = 0; l < n - i - 1; l++)
        {

            printf(" ");
        }
        printf("x");
        if (i > 0)
        {
            for (int l = 0; l < i * 2 - 1; l++)
            {
                if (i == n - 1)
                {
                    printf("x");
                }
                else
                {
                    printf(" ");
                }
            }
            printf("x");
        }

        printf("\n");
    }
}

void rectNum(int w, int h)
{
    int z = 0;
    for (int i = 0; i < h; i++)
    {
        for (int l = 0; l < w; l++)
        {

            if (i == 0 || i == h - 1)
            {
                printf("x");
            }
            else if (l == 0 || l == w - 1)
            {
                printf("x");
            }
            else
            {
                printf("%d", z);
                z = ++z % 10;
            }
        }
        printf("\n");
    }
}

int main()
{
    printf("Line:\n");
    line(5);
    printf("\n");

    printf("Column:\n");
    col(5);
    printf("\n");

    printf("Square:\n");
    square(5);
    printf("\n");

    printf("Rectangle:\n");
    rect(5, 9);
    printf("\n");

    printf("Ladder:\n");
    ladder(5, 9);
    printf("\n");

    printf("Square 2:\n");
    square2(5);
    printf("\n");

    printf("Right diagonal:\n");
    diagr(5);
    printf("\n");

    printf("Left diagonal:\n");
    diagl(5);
    printf("\n");

    printf("Triangle:\n");
    triangle(5);
    printf("\n");

    printf("Filled rectangle:\n");
    rectNum(5, 9);
    printf("\n");

    return 0;
}