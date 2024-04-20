#include "tasks.h"
#include <iostream>

template <typename AnyIterable>
static auto to_vec(const AnyIterable &iter)
{
    using Type = std::decay_t<decltype(*iter.begin())>;

    std::vector<Type> items;
    for (auto item : iter)
    {
        items.push_back(item);
    }
    return items;
}

int main()
{
    NumpyArray<int> array{{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}};

    auto it = array.iter()
                  .filter([](const int item)
                          { return item % 2 == 0; })
                  .filter([](const int item)
                          { return item > 5; });

    for (auto i : it)
    {
        std::cout << i << std::endl;
    }

    return 0;
}
