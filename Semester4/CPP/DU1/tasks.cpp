#include "tasks.h"

int math::fibonacci(int n)
{
    if (n < 0)
    {
        return 0;
    }

    int prev = 0;
    int current = 1;

    for (int i = 0; i < n; ++i)
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
        if ('A' < input[i] && input[i] < 'Z' && i > 0)
        {
            newLen++;
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

    if (len == 0)
    {
        return false;
    }

    std::uint32_t tempResult = 0;

    for (char c : input)
    {
        int num = c - '0';
        if (num > 9)
        {
            return false;
        }

        std::uint32_t newResult = tempResult * 10 + num;
        std::uint32_t backResult = (newResult - num) / 10;

        // check overflow
        if (backResult != tempResult || newResult < tempResult)
        {
            return false;
        }

        tempResult = newResult;
    }

    result = tempResult;
    return true;
}

bool strutils::validate_utf8(std::vector<std::uint8_t> input, size_t &result)
{
    size_t length = 0;
    std::int8_t nextSteps = 0;

    for (std::uint8_t c : input)
    {
        if (nextSteps > 0)
        {
            if (c >> 6 != 0b10)
            {
                return false;
            }

            nextSteps--;

            if (nextSteps == 0)
            {
                length++;
            }
            continue;
        }

        if (c >> 7 == 0b0)
        {
            // U+0000 - U+007F
            length++;
        }
        else if (c >> 5 == 0b110)
        {
            nextSteps = 1;
        }
        else if (c >> 4 == 0b01110)
        {
            nextSteps = 2;
        }
        else if (c >> 3 == 0b11110)
        {
            nextSteps = 3;
        }
        else
        {
            return false;
        }
    }

    if (nextSteps != 0)
    {
        return false;
    }

    result = length;
    return true;
}
