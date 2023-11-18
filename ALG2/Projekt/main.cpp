#include <iostream>
#include <string>
#include "HeightMap.hpp"

int main()
{
    HeightMap c("data.txt");

    std::cout << "Objekt má " << c.getFaces() << " stran." << std::endl;

    return 0;
}
