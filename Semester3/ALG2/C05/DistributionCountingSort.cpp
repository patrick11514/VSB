#include "DistributionCountingSort.h"

#include <fstream>
#include <iostream>

std::vector<int> loadFromFile(std::string fileName)
{
    std::ifstream file(fileName);

    if (!file.is_open())
    {
        std::cout << "File not found!" << std::endl;
        throw;
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
        std::cout << "File not found!" << std::endl;
        throw;
    }

    for (auto c : data)
    {
        file << (char)c;
    }

    file.close();
}

std::vector<int> distributionCountingSort(std::vector<int> data, int min, int max)
{
    if (min > max)
    {
        std::cout << "Min cannot be greater than max!" << std::endl;
        throw;
    }

    std::vector<int> counts(max - min);

    for (auto num : data)
    {
        counts[num - min]++;
    }

    std::vector<int> distributions(max - min);

    for (std::vector<int>::size_type i = 0; i < counts.size(); ++i)
    {
        if (i > 0)
        {
            distributions[i] = counts[i] + distributions[i - 1];
        }
        else
        {
            distributions[i] = counts[i];
        }
    }

    std::vector<int> output(data.size());

    for (auto num : data)
    {
        int j = num - min;
        int d = --distributions[j];

        output[d] = num;
    }

    return output;
}

std::vector<int> distributionCountingSortFromFile(std::string fileName, int min, int max)
{
    if (min > max)
    {
        std::cout << "Min cannot be greater than max!" << std::endl;
        throw;
    }

    std::ifstream file(fileName);

    if (!file.is_open())
    {
        std::cout << "File not found!" << std::endl;
        throw;
    }

    std::vector<int> counts(max - min);

    while (!file.eof())
    {
        char c;
        file >> c;
        counts[c - min]++;
    }

    file.close();

    std::vector<int> distributions(max - min);

    for (std::vector<int>::size_type i = 0; i < counts.size(); ++i)
    {
        if (i > 0)
        {
            distributions[i] = counts[i] + distributions[i - 1];
        }
        else
        {
            distributions[i] = counts[i];
        }
    }

    // size of output based of position of last char
    std::vector<int> output(distributions[counts.size() - 1]);

    // open file again and sort using it
    file.open(fileName);

    if (!file.is_open())
    {
        std::cout << "File not found!" << std::endl;
        throw;
    }

    while (!file.eof())
    {
        char c;
        file >> c;

        int j = (int)c - min;
        int d = --distributions[j];
        output[d] = c;
    }

    file.close();

    return output;
}