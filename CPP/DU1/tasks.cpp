#include "tasks.h"

int math::fibonacci(int n)
{
    if (n < 0)
    {
        return 0;
    }

    int prev = 0;
    int current = 1;

    for (int i = 0; i < n; i++)
    {
        int temp = current;
        current = prev + current;
        prev = temp;
    }

    return prev;
}

std::string strutils::camel_to_snake(std::string input)
{
    size_t len = input.length();
    size_t newLen = len;

    std::string newStr = "";

    for (size_t i = 0; i < len; ++i)
    {
        if ('A' < input[i] && input[i] < 'Z')
        {
            if (i > 0)
            {
                newLen++;
            }
        }
    }

    newStr.reserve(newLen);

    for (size_t i = 0; i < len; ++i)
    {
        char c = input[i];

        if (c < 'A' || c > 'Z')
        {
            newStr += c;
            continue;
        }

        char newC = c - 'A' + 'a';
        if (i > 0)
        {
            newStr += "_";
        }
        newStr += newC;
    }

    return newStr;
}

bool strutils::parse_uint(std::string input, std::uint32_t &result)
{
    size_t len = input.length();

    result = 0;

    for (size_t i = 0; i < len; ++i)
    {
        int num = input[i];
        if (num > 9)
        {
            return false;
        }

        result = result * 10 + num;
    }

    return true;
}
