#include "generator.hpp"
#include <random>

static const int min = -100;
static const int max = 100;

char getChar(int n)
{
    if (n == 0)
        return '*';
    if (n == 1)
        return '/';
    return '%';
}

void output(int t_L, int t_N, bool hex)
{
    for (int l = 0; l < t_L; ++l)
    {
        for (int i = 0; i < t_N; ++i)
        {
            int num = std::floor((std::rand() % (std::abs(max) + std::abs(min))) + min);

            char symbol;

            if (i == t_N - 1)
            {
                symbol = '=';
            }
            else
            {
                symbol = getChar(std::floor(std::rand() % 3));
            }

            if (hex)
            {
                printf("0x%X%c", num, symbol);
            }
            else
            {
                printf("%d%c", num, symbol);
            }
        }
        printf("\n");
    }
}