//***************************************************************************
//
// Program for education in subject "Assembly Languages"
// petr.olivka@vsb.cz, Department of Computer Science, VSB-TUO
//
// Empty project
//
//***************************************************************************

#include <stdio.h>

int g_int_pole[10] = {1, 12, 3, 4, 5, 6, 7, 8, 9, 10};
int g_suma;

void suma_pole();

char g_string[] = "Nazdar programatori!";
int g_strlen;

void get_len();

int g_max;

int search_max();

int main()
{
    suma_pole();
    printf("%d\n", g_suma);

    get_len();
    printf("%d\n", g_strlen);

    int l_max = search_max();
    printf("%d %d\n", g_max, l_max);
}
