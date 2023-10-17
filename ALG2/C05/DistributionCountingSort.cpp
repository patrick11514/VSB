#include "DistributionCountingSort.h"

#include <fstream>

std::vector<char> loadFromFile(std::string fileName)
{
    std::ifstream file(fileName);

    if (!file.is_open())
    {
        throw "File not found!";
    }

    std::vector<int> data;

    while (!file.eof())
    {
        char c;
        file >> c;
        data.push_back(c);
    }

    file.close();

    return data;
}

void putToFile(std::string fileName, std::vector<int> data)
{
    std::ofstream file(fileName);

    if (!file.is_open())
    {
        throw "File couldn't be opened!";
    }

    for (auto c : data)
    {
        file << c;
    }

    file.close();
}

std::vector<int> distributionCountingSort(std::vector<int> input, int min, int max)
{
    if (min > max)
    {
        throw "Min cannot be greater than max!";
    }

    std::vector<int> counts(max - min);

    for (std::vector<int>)
}