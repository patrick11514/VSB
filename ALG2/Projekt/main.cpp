#include <iostream>
#include <string>
#include "HeightMap.hpp"

int main()
{
    HeightMap c("data.txt");

    std::cout << c.getFaces() << std::endl;

    return 0;
}
