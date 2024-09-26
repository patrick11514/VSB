#include <iostream>
#include <string>
#include <ctime>
#include <random>
#include <string.h>

int main(int argc, char **argv)
{
    int binary = false;

    if (argc > 1 && strcmp(argv[1], "-b") == 0)
    {
        binary = true;
    }

    int num;
    int sum = 0;
    if (!binary)
    {
        while (scanf("%d", &num) != -1)
        {
            sum += num;
        }
    }
    else
    {
        int num;

        while (fread(&num, sizeof(num), 1, stdin))
        {
            sum += num;
        }
    }

    if (binary)
    {
        // fwrite(&sum, sizeof(sum), 1, stdout);
        //  return 0;
    }

    printf("%d", sum);
    return 0;
}
