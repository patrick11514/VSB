#pragma once

#include <string>
#include <cstdint>
#include <vector>

namespace math
{
    int fibonacci(int n);
}

namespace strutils
{

    std::string camel_to_snake(std::string input);
    bool parse_uint(std::string input, std::uint32_t &result);
    bool validate_utf8(std::vector<std::uint8_t> input, size_t &result);
}