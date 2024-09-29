#include <iostream>
#include <string>
#include <bitset>

using std::size_t;

int toInt(std::bitset<32> &bits)
{
    if (bits[31] == 1)
    {
        // should be negative
        return -1;
    }
    return static_cast<int>(bits.to_ulong());
}

int main()
{
    while (true)
    {
        std::string line;
        std::getline(std::cin, line);

        if (line.size() == 0)
        {
            break;
        }

        int number = std::stoi(line);
        std::bitset<32> bits(number);

        std::cout << "PRE" << std::endl;
        for (int i = 0; i < 32; ++i)
        {
            std::cout << bits[i];
        }
        std::endl(std::cout);
        std::cout << number << std::endl;

        for (int i = 0; i < 16; ++i)
        {
            bool tmp = bits[i];
            bits[i] = bits[31 - i];
            bits[31 - i] = tmp;
        }

        std::cout << "POST" << std::endl;
        for (int i = 0; i < 32; ++i)
        {
            std::cout << bits[i];
        }

        std::endl(std::cout);

        std::cout << toInt(bits) << std::endl;
    }

    return 0;
}
