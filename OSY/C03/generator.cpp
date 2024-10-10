#include <cstring>
#include <random>
#include <unistd.h>

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

void output(int t_L, int t_N /*, bool hex*/)
{
    int row = 1;
    while (true)
    {
        printf("%d. ", row);
        ++row;

        for (int i = 0; i < t_N; ++i)
        {
            int num = std::floor((std::rand() % (std::abs(max) + std::abs(min))) + min);

            // if (hex)
            //{
            //     printf("0x%X%c", num, symbol);
            // }
            // else
            //{
            printf("%d ", num);
            //}
        }
        printf("\n");
        fflush(stdout);
        usleep(60000000 / t_L);
    }
}

int main(int argc, char **argv)
{
    // bool hex = false;
    if (argc == 1)
        return -1;

    // if (argc == 4 && strcmp(argv[3], "-x") == 0)
    //     hex = true;

    int L = std::stoi(argv[1]);
    int N = std::stoi(argv[2]);

    output(L, N /*, hex*/);

    return 0;
}
