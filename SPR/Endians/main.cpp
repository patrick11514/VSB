#include <iostream>
#include <string>

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

        char *bytes = (char *)&number;
        char newBytes[4] = {0};

        for (int i = 0; i < 4; ++i)
        {
            newBytes[4 - i - 1] = bytes[i];
        }

        int result = *(int *)newBytes;
        std::cout << number << " converts to " << result << std::endl;
    }

    return 0;
}
