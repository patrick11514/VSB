#include <iostream>
#include <string>
#include "generator.hpp"

int main(int argc, char **argv)
{
    if (argc == 1)
        return -1;

    int L = std::stoi(argv[1]);
    int N = std::stoi(argv[2]);

    output(L, N);

    return 0;
}
