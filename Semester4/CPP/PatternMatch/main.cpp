#include <iostream>
#include <string>

int patternMatch(std::string text, std::string vzor)
{
    for (int i = 0; i <= static_cast<int>(text.length()) - static_cast<int>(vzor.length()); i++)
    {
        int j = 0;
        while (j < vzor.length())
        {
            if (vzor[j] != text[i + j])
            {
                break;
            }

            j++;
        }

        if (j == vzor.length())
        {
            return i;
        }
    }

    return -1;
}

int main()
{
    std::string vzor = "AHOMAHOJ";
    std::string text = "AHOJ";

    std::cout << patternMatch(text, vzor) << std::endl;

    return 0;
}
