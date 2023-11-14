#include <iostream>
#include <string>
#include <fstream>

int main()
{
    std::ifstream file("test.txt");

    if (!file.is_open())
    {
        std::cout << "File not found!" << std::endl;
        return 1;
    }

    while (!file.eof())
    {
        std::string line;
        std::getline(file, line);
        std::cout << line << std::endl;
    }

    return 0;
}
