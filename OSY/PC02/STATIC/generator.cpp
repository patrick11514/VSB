#include "generator.hpp"
#include <random>

static const int min = -1000;
static const int max = 1000;

void output(int t_L, int t_N)
{
    for (int l = 0; l < t_L; ++l)
    {
        int sum = 0;
        for (int i = 0; i < t_N; ++i)
        {
            int num = std::floor((std::rand() % (std::abs(max) + std::abs(min))) + min);

            printf("%d ", num);
            sum += num;
        }

        printf("%d\n", sum);
    }
}