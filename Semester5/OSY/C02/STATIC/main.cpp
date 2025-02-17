#include <iostream>
#include <string>
#include <cstring>
#include "generator.hpp"

int main(int argc, char **argv)
{
    bool hex = false;
    if (argc == 1)
        return -1;

    if (argc == 4 && strcmp(argv[3], "-x") == 0)
        hex = true;

    int L = std::stoi(argv[1]);
    int N = std::stoi(argv[2]);

    output(L, N, hex);

    return 0;
}
