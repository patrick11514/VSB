// This tells doctest to provide a main() function
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "tasks.h"

TEST_SUITE("UTF8String") {
    TEST_CASE("Create empty") {
        const UTF8String str;
        REQUIRE(str.get_byte_count() == 0);
        REQUIRE(str.get_point_count() == 0);
    }
    TEST_CASE("Create with string") {
        const UTF8String str{"hello"};
        REQUIRE(str.get_byte_count() == 5);
        REQUIRE(str.get_point_count() == 5);
    }
    TEST_CASE("Create with code points") {
        const UTF8String str{std::vector<CodePoint>{
            0x1F601, // 😁
            0x1F614, // 😔
            static_cast<CodePoint>('0'),
            0x010D // č
        }};
        REQUIRE(str.get_byte_count() == 11);
        REQUIRE(str.get_point_count() == 4);
    }
    TEST_CASE("String can be copied") {
        UTF8String str{std::vector<CodePoint>{
                0x1F601, // 😁
                0x1F614, // 😔
                static_cast<CodePoint>('0'),
                0x010D // č
        }};
        str.append(' ');
        UTF8String str2(str);

        REQUIRE(str.get_byte_count() == 12);
        REQUIRE(str.get_point_count() == 5);

        REQUIRE(str2.get_byte_count() == 12);
        REQUIRE(str2.get_point_count() == 5);

        REQUIRE(str == str2);

        UTF8String str3 = str;
        REQUIRE(str3 == str);
    }
    TEST_CASE("Non-empty string can be assigned") {
        UTF8String str{std::vector<CodePoint>{
                0x1F601, // 😁
                0x1F614, // 😔
                static_cast<CodePoint>('0'),
                0x010D // č
        }};
        str.append(' ');

        UTF8String str2{std::vector<CodePoint>{
            0x1F614,
            0x010D,
            static_cast<CodePoint>('f')
        }};
        str2.append('a');

        str = str2;

        REQUIRE(str2.get_byte_count() == 8);
        REQUIRE(str2.get_point_count() == 4);

        REQUIRE(str.get_byte_count() == 8);
        REQUIRE(str.get_point_count() == 4);

        REQUIRE(str == str2);
    }
    TEST_CASE("Empty string can be assigned") {
        UTF8String str2{std::vector<CodePoint>{
                0x1F614,
                0x010D,
                static_cast<CodePoint>('f')
        }};
        str2.append('a');

        UTF8String str;
        str = str2;

        REQUIRE(str2.get_byte_count() == 8);
        REQUIRE(str2.get_point_count() == 4);

        REQUIRE(str.get_byte_count() == 8);
        REQUIRE(str.get_point_count() == 4);

        REQUIRE(str == str2);
    }
    TEST_CASE("Self assignment") {
        UTF8String str{std::vector<CodePoint>{
                0x1F601, // 😁
                0x1F614, // 😔
                static_cast<CodePoint>('0'),
                0x010D // č
        }};
        str = str;

        REQUIRE(str.get_byte_count() == 11);
        REQUIRE(str.get_point_count() == 4);
    }
    TEST_CASE("Byte indexing") {
        const UTF8String str{std::vector<CodePoint>{
            static_cast<CodePoint>('A'),
            static_cast<CodePoint>('h'),
            static_cast<CodePoint>('o'),
            static_cast<CodePoint>('j'),
            static_cast<CodePoint>(' '),
            0x1F601, // 😁
            static_cast<CodePoint>('.'),
        }};

        REQUIRE(str.get_byte_count() == 10);
        REQUIRE(str.get_point_count() == 7);

        REQUIRE(str[0].value() == 'A');
        REQUIRE(str[1].value() == 'h');
        REQUIRE(str[2].value() == 'o');
        REQUIRE(str[3].value() == 'j');
        REQUIRE(str[4].value() == ' ');
        REQUIRE(str[5].value() == 0xF0);
        REQUIRE(str[6].value() == 0x9F);
        REQUIRE(str[7].value() == 0x98);
        REQUIRE(str[8].value() == 0x81);
        REQUIRE(str[9].value() == '.');
    }
    TEST_CASE("Code point indexing") {
        UTF8String string;
        string.append('A');
        string.append('h');
        string.append('o');
        string.append('j');
        string.append(' ');
        string.append(static_cast<CodePoint>(0x1F601));
        string.append('.');

        REQUIRE(string.get_byte_count() == 10);
        REQUIRE(string.get_point_count() == 7);

        const UTF8String& str = string;
        REQUIRE(str.nth_code_point(0).value() == 'A');
        REQUIRE(str.nth_code_point(1).value() == 'h');
        REQUIRE(str.nth_code_point(2).value() == 'o');
        REQUIRE(str.nth_code_point(3).value() == 'j');
        REQUIRE(str.nth_code_point(4).value() == ' ');
        REQUIRE(str.nth_code_point(5).value() == 0x1F601);
        REQUIRE(str.nth_code_point(6).value() == '.');
    }
    TEST_CASE("Invalid byte index") {
        UTF8String str{"foo"};
        REQUIRE(!str[4].has_value());
    }
    TEST_CASE("Invalid code point index") {
        UTF8String str;
        str.append('f');
        str.append(static_cast<CodePoint>(0x1F601));
        str.append('o');
        REQUIRE(!str.nth_code_point(4).has_value());
    }
    TEST_CASE("Append char") {
        UTF8String str{std::vector<CodePoint>{
            static_cast<CodePoint>('a'),
            0x1F601 // 😁
        }};
        str.append('a');
        REQUIRE(str.get_byte_count() == 6);
        REQUIRE(str.get_point_count() == 3);

        REQUIRE(str[5].value() == 'a');
    }
    TEST_CASE("Append char stress test") {
        UTF8String str;

        size_t count = 10'000;
        for (size_t i = 0; i < count; i++) {
            str.append('a');
        }
        REQUIRE(str.get_byte_count() == count);
        REQUIRE(str.get_point_count() == count);

        REQUIRE(str == UTF8String{std::vector<CodePoint>(count, static_cast<CodePoint>('a'))});
    }
    TEST_CASE("Append code point 1") {
        UTF8String str{std::vector<CodePoint>{
                static_cast<CodePoint>('a'),
                0x1F601 // 😁
        }};
        str.append(static_cast<CodePoint>('b'));
        str.append(static_cast<CodePoint>(0x1F602)); // 😂

        REQUIRE(str.get_byte_count() == 10);
        REQUIRE(str.get_point_count() == 4);

        REQUIRE(str[0].value() == 'a');
        REQUIRE(str[5].value() == 'b');
        REQUIRE(str[6].value() == 0xF0);
        REQUIRE(str[7].value() == 0x9F);
        REQUIRE(str[8].value() == 0x98);
        REQUIRE(str[9].value() == 0x82);
    }
    TEST_CASE("Append code point 2") {
        UTF8String str;
        str.append(static_cast<CodePoint>('a'));
        str.append(static_cast<CodePoint>(0x00A3));  // £
        str.append(static_cast<CodePoint>(0x20AC));  // €
        str.append(static_cast<CodePoint>(0x10348)); // 𐍈

        REQUIRE(str.get_byte_count() == 10);
        REQUIRE(str.get_point_count() == 4);
    }
    TEST_CASE("Append code point stress test") {
        UTF8String str;

        size_t count = 1'000;
        for (size_t i = 0; i < count; i++) {
            str.append(static_cast<CodePoint>(0x20AC));  // €
        }
        REQUIRE(str.get_byte_count() == count * 3);
        REQUIRE(str.get_point_count() == count);

        REQUIRE(str == UTF8String{std::vector<CodePoint>(count, 0x20AC)});
    }
    TEST_CASE("Join strings 1") {
        UTF8String str{"Hello"};
        UTF8String str2{" world"};

        REQUIRE((str + str2) == UTF8String{"Hello world"});
        REQUIRE(str == UTF8String{"Hello"});
        REQUIRE(str2 == UTF8String{" world"});
    }
    TEST_CASE("Join strings 2") {
        UTF8String str{"Hello"};

        REQUIRE((str + UTF8String{}) == UTF8String{"Hello"});
        REQUIRE((UTF8String{} + str) == UTF8String{"Hello"});
    }
    TEST_CASE("Join strings 3") {
        UTF8String str{"Hello"};
        UTF8String str2{std::vector<CodePoint>{
            0x20AC,  // €
            static_cast<CodePoint>('f')
        }};

        auto res1 = str + str2;
        REQUIRE(res1.get_byte_count() == 9);
        REQUIRE(res1.get_point_count() == 7);

        auto res2 = str2 + str;
        REQUIRE(res2.get_byte_count() == 9);
        REQUIRE(res2.get_point_count() == 7);
    }
    TEST_CASE("Append strings 1") {
        UTF8String str{"Hello"};
        UTF8String str2{", "};
        UTF8String str3{"World"};
        UTF8String str4{"!"};

        str += str2;
        str += str3;
        str += str4;

        REQUIRE(str == UTF8String{"Hello, World!"});
    }
    TEST_CASE("Append strings 2") {
        UTF8String str{"Hello"};
        UTF8String str2{std::vector<CodePoint>{
            0x20AC,  // €
            static_cast<CodePoint>('f')
        }};

        str += str2;
        REQUIRE(str.get_byte_count() == 9);
        REQUIRE(str.get_point_count() == 7);
    }
    TEST_CASE("Test cast") {
        UTF8String str{"Ahoj"};

        std::string str2 = static_cast<std::string>(str);
        REQUIRE(str2 == "Ahoj");
    }
}
