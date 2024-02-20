#pragma once

#include <string>
#include <cstdint>

namespace math
{
    int fibonacci(int n);
}

namespace strutils
{

    std::string camel_to_snake(std::string input);
    bool parse_uint(std::string input, std::uint32_t &result);
}