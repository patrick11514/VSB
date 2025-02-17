#include <iostream>
#include <string>
#include <vector>

enum class Color
{
    Red,
    Blue,
    Orange
};

enum class Aircraft
{
    A350,
    A330
};

struct Bitfield
{
    int a : 3;
    int b : 3;
};

// struct = product type
// union = sum type, dokáže tam být pouze string, nebo int
union Union
{
    std::string str;
    int i;
};

class Test
{
public:
    std::vector<char> data;

    operator bool() const;
};

Test::operator bool() const
{
    if (data.size() == 0)
    {
        return false;
    }
    return true;
}

int main()
{
    Test a;
    Test b;
    b.data.push_back('a');

    if (a)
    {
        std::cout << "A full" << std::endl;
    }
    else
    {
        std::cout << "A empty" << std::endl;
    }

    if (b)
    {
        std::cout << "B full" << std::endl;
    }
    else
    {
        std::cout << "B empty" << std::endl;
    }
    return 0;
}
