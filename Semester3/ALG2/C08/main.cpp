#include <iostream>
#include <string>
#include "DisjointSubsets.h"

int main()
{
    std::vector<std::string> names = {"Alpha", "Bravo", "Charlie", "Delta", "Echo", "Foxtrot"};

    DisjointSubsets ds(names);

    for (const std::string &n : names)
    {
        std::cout << n << " -> " << ds.find(n) << std::endl;
    }

    ds.Union(names[0], names[2]);
    ds.Union(names[0], names[4]);
    ds.Union(names[5], names[4]);
    ds.Union(names[1], names[3]);
    ds.Union(names[1], names[5]);

    std::cout << std::endl;

    for (const std::string &n : names)
    {
        std::cout << n << " -> " << ds.find(n) << std::endl;
    }

    std::cout << std::endl;

    return 0;
}
