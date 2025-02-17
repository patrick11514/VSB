#include <iostream>
#include <fstream>
#include <chrono>

#include "Collection.hpp"

int main()
{
    auto start = std::chrono::high_resolution_clock::now();

    Collection coll("../input.txt");

    std::cout << "Původních záznamů: " << coll.inputCount << std::endl;
    std::cout << "Neduplicitních záznamů: " << coll.getCurrentCount() << std::endl;

    std::cout << "Min Temp: " << coll.getMinTemp() << std::endl;
    std::cout << "Max Temp: " << coll.getMaxTemp() << std::endl;

    std::cout << "Min Humidity: " << coll.getMinHumidity() << std::endl;
    std::cout << "Max Humidity: " << coll.getMaxHumidity() << std::endl;

    // std::cout << "Temp Histogram: " << std::endl;
    // coll.printTempHistorgram();
    // std::cout << "Humidity Historgam: " << std::endl;
    // coll.printHumidityHistorgram();
    // coll.toFile("../output.txt");

    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "Took: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start) << std::endl;
}
