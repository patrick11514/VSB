#include "ComparsionCountingSort.h"

std::vector<int> comparsionCountingSort(std::vector<int> input)
{
    std::vector<int> count(input.size());
    std::vector<int> out(input.size());

    for (std::vector<int>::size_type i = 0; i < input.size() - 1; i++)
    {
        for (std::vector<int>::size_type j = i + 1; j < input.size(); j++)
        {
            if (input[i] < input[j])
            {
                count[j]++;
            }
            else
            {
                count[i]++;
            }
        }
    }

    for (std::vector<int>::size_type i = 0; i < input.size(); i++)
    {
        out[count[i]] = input[i];
    }

    return out;
}