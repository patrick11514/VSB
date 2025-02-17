#include <iostream>
#include <string>
#include <vector>
#include <iostream>
#include "DistributionCountingSort.h"

int main()
{
    // classic Distribution Sorting
    // load data from file to memory and sort it
    {
        std::vector<int> chars = loadFromFile("Ecoli.txt");

        std::vector<int>
            sorted = distributionCountingSort(chars, 'a', 'z');

        putToFile("out-classic.txt", sorted);
    }

    // edited for our case
    // load data from file and sort it without storing data to memory
    {
        std::vector<int> sorted = distributionCountingSortFromFile("Ecoli.txt", 'a', 'z');

        putToFile("out-fromFile.txt", sorted);
    }

    return 0;
}
