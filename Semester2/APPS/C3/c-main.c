//***************************************************************************
//
// Program for education in subject "Assembly Languages"
// petr.olivka@vsb.cz, Department of Computer Science, VSB-TUO
//
// Empty project
//
//***************************************************************************

#include <stdio.h>

int l_first[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
int l_second[] = {10, 10, 10, 10, 10, 10, 10, 10, 10, 10};
int l_sum[10];

void sum_arrays(int *first, int *second, int *sum, int length);

char l_msg_one[] = "Ahoj, toto je";
char l_msg_two[] = " kompletni zprava.";
char l_concat[32];

int concat_strings(char *first_str, char *second_str, char *concat_str);

int l_number = 0xF0F0F0F0;
char l_number_str[35];

void num_to_bin_str(int number, char *hex_str);

int main()
{
    sum_arrays(l_first, l_second, l_sum, 10);
    for (int i = 0; i < 10; i++)
    {
        printf("%d ", l_sum[i]);
    }
    printf("\n");

    int len = concat_strings(l_msg_one, l_msg_two, l_concat);
    // printf("Len: %d\n", len);
    printf("%s\n", l_concat);

    num_to_bin_str(l_number, l_number_str);
    printf("%s\n", l_number_str);
}
