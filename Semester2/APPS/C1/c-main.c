//***************************************************************************
//
// Program for education in subject "Assembly Languages"
// petr.olivka@vsb.cz, Department of Computer Science, VSB-TUO
//
// Empty project
//
//***************************************************************************

#include <stdio.h>

char g_string[] = "Tcnto radck je bcz chyby.";
void fix_text();

int g_number = 0x12345678;
void change_endianity();

int g_int = -10;
long g_long = 0;
void transfer_number();

int main()
{
    printf("%s\n", g_string);
    fix_text();
    printf("%s\n", g_string);

    printf("0x%x\n", g_number);
    change_endianity();
    printf("0x%x\n", g_number);

    printf("%ld\n", g_long);
    transfer_number();
    printf("%ld\n", g_long);
}
