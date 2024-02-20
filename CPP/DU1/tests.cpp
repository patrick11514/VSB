// This tells doctest to provide a main() function
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "tasks.h"

// Note: comment out the tests to avoid lots of compile errors that will occur before you
// implement the functions. Uncomment the tests for the function that you are currently implementing.

TEST_CASE("Fibonacci")
{
    REQUIRE(math::fibonacci(0) == 0);
    REQUIRE(math::fibonacci(1) == 1);
    REQUIRE(math::fibonacci(2) == 1);
    REQUIRE(math::fibonacci(3) == 2);
    REQUIRE(math::fibonacci(4) == 3);
    REQUIRE(math::fibonacci(5) == 5);
    REQUIRE(math::fibonacci(6) == 8);
    REQUIRE(math::fibonacci(40) == 102334155);
}

TEST_SUITE("Camel to snake conversion")
{
    TEST_CASE("Empty string")
    {
        REQUIRE(strutils::camel_to_snake("") == "");
    }
    TEST_CASE("Single word")
    {
        REQUIRE(strutils::camel_to_snake("foo") == "foo");
    }
    TEST_CASE("Two words")
    {
        REQUIRE(strutils::camel_to_snake("fooBar") == "foo_bar");
    }
    TEST_CASE("Multiple words")
    {
        REQUIRE(strutils::camel_to_snake("fooBarBaz") == "foo_bar_baz");
    }
    TEST_CASE("Short words")
    {
        REQUIRE(strutils::camel_to_snake("aBCDEFGxH") == "a_b_c_d_e_f_gx_h");
    }
    TEST_CASE("Long word")
    {
        REQUIRE(strutils::camel_to_snake("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA") == "a_a_a_a_a_a_a_a_a_a_a_a_a_a_a_a_a_a_a_a_a_a_a_a_a_a_a_a_a_a_a_a_a_a_a_a_a_a_a_a");
    }
}

TEST_SUITE("Integer parsing")
{
    TEST_CASE("Empty string")
    {
        uint32_t result = std::numeric_limits<uint32_t>::max();
        REQUIRE(!strutils::parse_uint("", result));
    }
    TEST_CASE("Zero")
    {
        uint32_t result = std::numeric_limits<uint32_t>::max();
        REQUIRE(strutils::parse_uint("0", result));
        REQUIRE(result == 0);
    }
    TEST_CASE("Single digit")
    {
        uint32_t result = std::numeric_limits<uint32_t>::max();
        REQUIRE(strutils::parse_uint("5", result));
        REQUIRE(result == 5);
    }
    TEST_CASE("Two digits")
    {
        uint32_t result = std::numeric_limits<uint32_t>::max();
        REQUIRE(strutils::parse_uint("99", result));
        REQUIRE(result == 99);
    }
    TEST_CASE("Two digits")
    {
        uint32_t result = std::numeric_limits<uint32_t>::max();
        REQUIRE(strutils::parse_uint("99", result));
        REQUIRE(result == 99);
    }
    TEST_CASE("Large number")
    {
        uint32_t result = std::numeric_limits<uint32_t>::max();
        REQUIRE(strutils::parse_uint("123456789", result));
        REQUIRE(result == 123456789);
    }
    TEST_CASE("Maximum")
    {
        uint32_t result = std::numeric_limits<uint32_t>::max();
        REQUIRE(strutils::parse_uint("4294967295", result));
        REQUIRE(result == 4294967295);
    }
    TEST_CASE("Overflow")
    {
        uint32_t result = std::numeric_limits<uint32_t>::max();
        REQUIRE(!strutils::parse_uint("4294967296", result));
    }
    TEST_CASE("Overflow 2")
    {
        uint32_t result = std::numeric_limits<uint32_t>::max();
        REQUIRE(!strutils::parse_uint("4294967297", result));
    }
    TEST_CASE("Overflow 3")
    {
        uint32_t result = std::numeric_limits<uint32_t>::max();
        REQUIRE(!strutils::parse_uint("999999999999999", result));
    }
    TEST_CASE("Overflow 4")
    {
        uint32_t result = std::numeric_limits<uint32_t>::max();
        REQUIRE(!strutils::parse_uint("9223372036854775808", result));
    }
    TEST_CASE("Overflow 5")
    {
        uint32_t result = std::numeric_limits<uint32_t>::max();
        REQUIRE(!strutils::parse_uint("5000000000", result));
    }
    TEST_CASE("Do not change result when parsing fails")
    {
        uint32_t result = 123;
        REQUIRE(!strutils::parse_uint("4294967297", result));
        REQUIRE(result == 123);
    }
    TEST_CASE("Invalid prefix")
    {
        uint32_t result;
        REQUIRE(!strutils::parse_uint("   123456789", result));
    }
    TEST_CASE("Invalid character in the middle")
    {
        uint32_t result;
        REQUIRE(!strutils::parse_uint("1234x56789", result));
    }
    TEST_CASE("Invalid postfix")
    {
        uint32_t result;
        REQUIRE(!strutils::parse_uint("123456789 x", result));
    }
}

TEST_SUITE("UTF-8 validation")
{
    TEST_CASE("Empty")
    {
        std::vector<uint8_t> input{};
        size_t result = std::numeric_limits<size_t>::max();
        REQUIRE(strutils::validate_utf8(input, result));
        REQUIRE(result == 0);
    }
    TEST_CASE("Simple")
    {
        std::vector<uint8_t> input{
            0b01100001,
            0b01101000,
            0b01101111,
            0b01101010};
        size_t result = std::numeric_limits<size_t>::max();
        REQUIRE(strutils::validate_utf8(input, result));
        REQUIRE(result == 4);
    }
    TEST_CASE("Complex points")
    {
        std::vector<uint8_t> input{
            0b01001100,
            0b11000011,
            0b10101101,
            0b01100010,
            0b11000011,
            0b10101101,
            0b00100000,
            0b01110011,
            0b01100101,
            0b00100000,
            0b01110100,
            0b01101001,
            0b00100000,
            0b01000011,
            0b00101011,
            0b00101011,
            0b00111111,
            0b00100000,
            0b11110000,
            0b10011111,
            0b10011000,
            0b10000000};
        size_t result = std::numeric_limits<size_t>::max();
        REQUIRE(strutils::validate_utf8(input, result));
        REQUIRE(result == 17);
    }
    TEST_CASE("Invalid input 1")
    {
        std::vector<uint8_t> input{0b11111111};
        size_t result = std::numeric_limits<size_t>::max();
        REQUIRE(!strutils::validate_utf8(input, result));
    }
    TEST_CASE("Invalid input 2")
    {
        std::vector<uint8_t> input{0b00001111, 0b10011001};
        size_t result = std::numeric_limits<size_t>::max();
        REQUIRE(!strutils::validate_utf8(input, result));
    }
    TEST_CASE("Invalid input 3")
    {
        std::vector<uint8_t> input{0b11000110, 0b10011001, 0b10010000};
        size_t result = std::numeric_limits<size_t>::max();
        REQUIRE(!strutils::validate_utf8(input, result));
    }
    TEST_CASE("Invalid input 4")
    {
        std::vector<uint8_t> input{0b11000110};
        size_t result = std::numeric_limits<size_t>::max();
        REQUIRE(!strutils::validate_utf8(input, result));
    }
    TEST_CASE("Invalid input 5")
    {
        std::vector<uint8_t> input{0b11000110, 0b00110110};
        size_t result = std::numeric_limits<size_t>::max();
        REQUIRE(!strutils::validate_utf8(input, result));
    }
    TEST_CASE("Invalid input 6")
    {
        std::vector<uint8_t> input{0b10110110};
        size_t result = std::numeric_limits<size_t>::max();
        REQUIRE(!strutils::validate_utf8(input, result));
    }
    TEST_CASE("Do not modify result on invalid input")
    {
        std::vector<uint8_t> input{0b10110110};
        size_t result = std::numeric_limits<size_t>::max();
        REQUIRE(!strutils::validate_utf8(input, result));
        REQUIRE_EQ(result, std::numeric_limits<size_t>::max());
    }
}
