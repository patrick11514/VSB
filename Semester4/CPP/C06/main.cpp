#include <iostream>
#include <string>
#include <format>

int main()
{
    std::string str = "123";
    std::string real = "12.3";

    std::cout << std::stoi(str) << std::endl;
    std::cout << std::stol(str) << std::endl;
    std::cout << std::stoul(str) << std::endl;
    std::cout << std::stof(real) << std::endl;
    std::cout << std::stod(real) << std::endl;

    std::cout << std::format("{}: {}\n", 1, "cool");

    return 0;
}
