#include <iostream>
#include <string>
#include <vector>
#include <iostream>

#include "ComparsionCountingSort.h"
#include "DistributionCountingSort.h"

int main()
{
    std::vector<int> numbers;
    numbers.push_back(62);
    numbers.push_back(31);
    numbers.push_back(84);
    numbers.push_back(96);
    numbers.push_back(19);
    numbers.push_back(47);

    std::vector<int> result = comparsionCountingSort(numbers);

    std::cout << "Input: " << std::endl;
    for (std::vector<int>::size_type i = 0; i < numbers.size(); i++)
    {
        std::cout << numbers[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "Comparsion Sort: " << std::endl;
    for (std::vector<int>::size_type i = 0; i < result.size(); i++)
    {
        std::cout << result[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "Distribution Sort: " << std::endl;
    for (std::vector<int>::size_type i = 0; i < result.size(); i++)
    {
        std::cout << result[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}
