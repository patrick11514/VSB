#include "tasks.h"
#include <iostream>

size_t index(std::vector<size_t> sizes, std::vector<size_t> indexes)
{
    size_t index = 0;

    for (size_t i = 0; i < sizes.size(); ++i)
    {
        size_t n = 1;
        for (size_t l = i + 1; l < sizes.size(); ++l)
        {
            // std::cout << sizes[l] << "*";
            n *= sizes[l];
        }

        // std::cout << indexes[i] << std::endl;

        index += n * indexes[i];
    }

    return index;
}

int main()
{
    std::vector<size_t> sizes{2, 4, 3, 3};
    for (size_t i = 0; i < 2; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            for (size_t k = 0; k < 3; k++)
            {
                for (size_t l = 0; l < 3; l++)
                {
                    std::vector<size_t> in{i, j, k, l};
                    std::cout << index(sizes, in) << std::endl;
                    // 0 0 0 = 0 // 3 * 4 * 0 + 3 * 0 + 0
                    // 0 0 1 = 1 // 3 * 4 * 0 + 3 * 0 + 1
                    // 0 0 2 = 2 // 3 * 4 * 0 + 3 * 0 + 2
                    // 0 1 0 = 3 // 3 * 4 * 0 + 3 * 1 + 0
                    // 0 1 1 = 4 // 3 * 4 + 0 + 3 * 1 + 1
                    // 0 1 2 = 5 // 3 * 4 + 0 + 3 * 1 + 2
                    // 0 2 0 = 6 // 3 * 4 + 0 + 3 * 2 + 0
                    // 0 2 1 = 7 //
                }
            }
        }
    }

    return 0;
}
