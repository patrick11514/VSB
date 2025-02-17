#include <iostream>
#include <string>
#include <regex>
#include <functional>

std::vector<int> map(const std::vector<int> &v, const std::function<int(int)> &fn)
{
    std::vector<int> dest;
    dest.reserve(v.size());

    for (const int i : v)
    {
        dest.emplace_back(fn(i));
    }

    return dest;
}

int main()
{
    // R"()" - RAW string, neřeší escaping
    std::regex re(R"([a-zA-Z]{2,4}\d{3})");
    std::cmatch m;
    if (std::regex_match("abcd123", m, re))
    {
        std::cout << "Found" << std::endl;
    }
    else
    {
        std::cout << "Not found" << std::endl;
    }

    struct Callable
    {
        int operator()(int x)
        {
            return x + 1;
        }
    };

    Callable callable;

    std::vector<int> v{1, 2, 3, 4, 5};

    auto c = [y = 5, v = std::move(v)](int x) -> int
    {
        for (int i : v)
        {
            std::cout << i << std::endl;
        }
        return x + y;
    };

    std::function<int(int)> e = c;

    std::cout << e(37) << std::endl;

    auto vector = map(std::vector<int>{4, 5, 6}, callable);

    for (int i : vector)
    {
        std::cout << i << std::endl;
    }

    return 0;
}
