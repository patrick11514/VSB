#include <iostream>
#include <sstream>

#include "decode.hpp"

bool isValid(char c)
{
    return (c >= '0' && c <= '9') ||
           (c >= 'a' && c <= 'f') ||
           (c >= 'A' && c <= 'F');
}

fs::path decode(const std::string_view &path)
{
    std::string newPath;
    newPath.reserve(path.size());

    bool isChar = false;

    for (auto it = path.begin(); it != path.end(); ++it)
    {
        auto c = *it;

        if (isChar == true)
        {
            char c1 = *it;
            char c2 = *(it + 1);

            if (!isValid(c1) || !isValid(c2))
            {
                // reset and process it as normal char
                newPath += "%";
                isChar = false;
                it--;
                continue;
            }

            std::string chars(it, it + 2);

            newPath += static_cast<char>(std::stol(chars, nullptr, 16));

            it++;
            isChar = false;

            continue;
        }

        if (c == '%')
        {
            isChar = true;
            continue;
        }

        newPath += c;
    }

    return newPath;
}