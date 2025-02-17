#include <vector>
#include <iostream>

int main()
{
    std::vector<int> vec{1, 2, 3, 4, 5, 6};

    for (auto zacatek = vec.begin(); zacatek != vec.end(); zacatek++)
    {
        int hodnota = *zacatek;
        std::cout << hodnota << std::endl;
    }

    for (auto hodnota : vec)
    {
        std::cout << hodnota << std::endl;
    }

    return 0;
}
/*
#include <vector>
#include <iostream>

int main()
{
    std::vector<int, std::allocator<int>> vec = std::vector<int, std::allocator<int>>{std::initializer_list<int>{1, 2, 3, 4, 5, 6}, std::allocator<int>()};
    {
        std::vector<int, std::allocator<int>> &__range1 = vec;
        for (__gnu_cxx::__normal_iterator<int *, std::vector<int, std::allocator<int>>> __begin1 = __gnu_cxx::__normal_iterator<int *, std::vector<int, std::allocator<int>>>(__range1.begin()), __end1 = __gnu_cxx::__normal_iterator<int *, std::vector<int, std::allocator<int>>>(__range1.end()); __gnu_cxx::operator!=(__begin1, __end1); __begin1.operator++())
        {
            int hodnota = __begin1.operator*();
            std::cout.operator<<(hodnota).operator<<(std::endl);
        }
    }
    return 0;
}*/
