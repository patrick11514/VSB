//***************************************************************************
//
// Program for education in subject "Assembly Languages"
// petr.olivka@vsb.cz, Department of Computer Science, VSB-TUO
//
// Empty project
//
//***************************************************************************

#include <stdio.h>

void testik(int a, int b, int c)
{
    printf(" a %d b %d c %d\n", a, b, c);
}

int suma_int(int a, int b);

int suma_pole(int *tp_pole, int t_N);

int pocet_cislic(char *pole);

long max_pole(long *tp_pole, int t_N);

int main()
{
    // int i = 0;
    // testik(i++, i++, i++);

    printf("suma: %d\n", suma_int(10, 19));

    int pole[10] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    printf("suma: %d\n", suma_pole(pole, 10));

    printf("pocet cisel: %d\n", pocet_cislic("Ah0j pr0gr4m4tor1!"));

    long long_pole[10] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    printf("max z pole: %ld\n", max_pole(long_pole, 10));
}
