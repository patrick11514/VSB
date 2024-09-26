#include <iostream>
#include <string>
#include <ctime>
#include <random>
#include <string.h>

int main(int argc, char **argv)
{
    if (argc == 1 || argc > 5)
    {
        printf("Invalid argument count.");
        return -1;
    }

    // srand(time(nullptr));

    bool binary = false;
    int offset = 0;

    if (strcmp(argv[1], "-b") == 0)
    {
        offset = 1;
        binary = true;
    }
    else if (argc == 3 && strcmp(argv[2], "-b") == 0)
    {
        binary = true;
    }
    else if (argc == 5 && strcmp(argv[4], "-b") == 0)
    {
        binary = true;
    }

    int count = std::stoi(argv[1 + offset]);

    if (argc == 2 || argc == 3)
    {
        for (int i = 0; i < count; ++i)
        {
            int num = rand();

            if (binary)
            {
                fwrite(&num, sizeof(num), 1, stdout);
                continue;
            }

            printf("%d\n", num);
        }
        return 0;
    }

    int min = std::stoi(argv[2 + offset]);
    int max = std::stoi(argv[3 + offset]);

    for (int i = 0; i < count; ++i)
    {
        int num = rand() % max + min;

        if (binary)
        {
            fwrite(&num, sizeof(num), 1, stdout);
            continue;
        }

        printf("%d\n", num);
    }

    return 0;
}
