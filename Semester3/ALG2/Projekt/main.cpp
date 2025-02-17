/**
 * @author Patrik Mintěl
 * @date 21.11.2023
 * @version 0.3.0
 * @link https://patrick115.eu
 */

#include <iostream>
#include <string>
#include <chrono>
#include "HeightMap.hpp"

int main()
{
    HeightMap c("data.txt");

    auto start = std::chrono::high_resolution_clock::now();
    std::cout << "Objekt má " << c.getFaces() << " stran." << std::endl;
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Výpočet stran: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;

    return 0;
}
