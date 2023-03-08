//***************************************************************************
//
// Program for education in subject "Assembly Languages"
// petr.olivka@vsb.cz, Department of Computer Science, VSB-TUO
//
// Empty project
//
//***************************************************************************

#include <stdio.h>

char g_string[] = "T_nto rad_k j_ b_z chyby.";

void fixtext();

long g_longs[] = {2, 3, 15, 16, 100, 105};

void doubleit();

int g_ints[] = {1, 2, 3, 4, 5, 6, 7, 8, 16};
int g_count;

void count();

int main()
{
    printf("%s\n", g_string);
    fixtext();
    printf("%s\n", g_string);

    for (int i = 0; i < 6; i++)
    {
        printf("%ld ", g_longs[i]);
    }
    printf("\n");
    doubleit();
    for (int i = 0; i < 6; i++)
    {
        printf("%ld ", g_longs[i]);
    }
    printf("\n");

    count();
    printf("%d\n", g_count);
}
