#include "tasks.h"

NumpyArray<bool> operator&&(bool value, const NumpyArray<bool> &array)
{
    std::vector<bool> newData;
    newData.reserve(array._size());

    for (size_t i = 0; i < array._size(); ++i)
    {
        newData.emplace_back(array[i] && value);
    }
    return NumpyArray(newData);
}