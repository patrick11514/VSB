//***************************************************************************
//
// Program for education in subject "Assembly Languages"
// petr.olivka@vsb.cz, Department of Computer Science, VSB-TUO
//
// Empty project
//
//***************************************************************************

#include <stdio.h>

int l_input_ints[] = {1, -2, 3, -4, 5, -6, 7, -8, 9, -10};
long l_tripled[10];

void triple_nums(int *input_array, long *output_array, int array_len);

int l_ints[] = {30, 41, 22, 21, 10, 6, 7, 55, 99, 60};
int k = 3;

int make_divisible(int *array, int array_len, int k);

int l_seq_index = 12;

int fibonacci(int seq_index);

long l_birth_num = 7506191528;
long l_birth_num_wrong = 7506191530;

int is_bnum_valid(long birth_num);

int main()
{
    triple_nums(l_input_ints, l_tripled, 10);
    for (int i = 0; i < 10; i++)
    {
        printf("%ld ", l_tripled[i]);
    }
    printf("\n");

    make_divisible(l_ints, 10, k);
    for (int i = 0; i < 10; i++)
    {
        printf("%d ", l_ints[i]);
    }
    printf("\n");

    printf("%d\n", fibonacci(0));

    printf("%d\n", is_bnum_valid(l_birth_num));
    printf("%d\n", is_bnum_valid(l_birth_num_wrong));
}
