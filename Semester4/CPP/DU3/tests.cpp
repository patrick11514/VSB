// This tells doctest to provide a main() function
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "tasks.h"

namespace
{

    auto parse_digit = any_of(std::vector<Parser>{
        create_char_parser('0'),
        create_char_parser('1'),
        create_char_parser('2'),
        create_char_parser('3'),
        create_char_parser('4'),
        create_char_parser('5'),
        create_char_parser('6'),
        create_char_parser('7'),
        create_char_parser('8'),
        create_char_parser('9')});

}

TEST_CASE("Parse char")
{
    auto parser1 = create_char_parser('x');

    REQUIRE(parser1("") == nullptr);
    REQUIRE(parser1("a") == nullptr);

    auto a = "x";
    REQUIRE(parser1(a) == a + 1);

    auto b = "xx";
    REQUIRE(parser1(b) == b + 1);
}

TEST_CASE("Parse word single character")
{
    auto parser1 = create_word_parser("x");

    REQUIRE(parser1("") == nullptr);
    REQUIRE(parser1("a") == nullptr);

    auto a = "x";
    REQUIRE(parser1(a) == a + 1);

    auto b = "xx";
    REQUIRE(parser1(b) == b + 1);
}

TEST_CASE("Parse word multiple characters")
{
    auto parser1 = create_word_parser("foo");

    REQUIRE(parser1("") == nullptr);
    REQUIRE(parser1("a") == nullptr);
    REQUIRE(parser1("f") == nullptr);
    REQUIRE(parser1("fo") == nullptr);
    REQUIRE(parser1("foa") == nullptr);
    REQUIRE(parser1("fo0ooo") == nullptr);

    auto a = "foo";
    REQUIRE(parser1(a) == a + 3);

    auto b = "foox";
    REQUIRE(parser1(b) == b + 3);
}

TEST_CASE("Parse any of")
{
    auto parser1 = any_of(std::vector<Parser>{
        parse_digit,
        create_char_parser('x'),
        create_word_parser("foo")});

    REQUIRE(parser1("") == nullptr);

    auto a = "x";
    REQUIRE(parser1(a) == a + 1);

    auto b = "1";
    REQUIRE(parser1(b) == b + 1);

    auto c = "y";
    REQUIRE(parser1(c) == nullptr);

    auto d = "foobar";
    REQUIRE(parser1(d) == d + 3);

    auto e = "12";
    REQUIRE(parser1(e) == e + 1);

    auto parser2 = any_of(std::vector<Parser>{
        create_word_parser("abc"),
        skip_ws()});

    auto f = "abc   ";
    REQUIRE(parser2(f) == f + 3);

    auto g = "   abc";
    REQUIRE(parser2(g) == g + 3);
}

TEST_CASE("Parse sequence")
{
    auto parser1 = sequence(std::vector<Parser>{
        parse_digit,
        create_char_parser('x'),
        parse_digit,
        create_word_parser("end")});

    REQUIRE(parser1("") == nullptr);
    REQUIRE(parser1("1") == nullptr);
    REQUIRE(parser1("1x") == nullptr);
    REQUIRE(parser1("1x1") == nullptr);
    REQUIRE(parser1("1x1en") == nullptr);
    REQUIRE(parser1("1x1foo") == nullptr);
    REQUIRE(parser1("x1x2end") == nullptr);

    auto a = "1x1endbar";
    REQUIRE(parser1(a) == a + 6);

    auto parser2 = sequence(std::vector<Parser>{
        create_word_parser("foo"),
        create_word_parser("oob"),
        create_word_parser("oba"),
    });
    REQUIRE(parser2("") == nullptr);
    REQUIRE(parser2("foo") == nullptr);
    REQUIRE(parser2("foooob") == nullptr);
    REQUIRE(parser2("foooobob") == nullptr);
    REQUIRE(parser2("ooboba") == nullptr);
    REQUIRE(parser2("fooba") == nullptr);

    auto b = "fooooboba";
    REQUIRE(parser2(b) == b + 9);

    auto parser3 = sequence(std::vector<Parser>{
        create_word_parser("vsb"),
        skip_ws(),
        create_char_parser('-'),
        skip_ws(),
        create_word_parser("tuo")});

    auto c = "vsb-tuo";
    auto d = "vsb   -tuo";
    auto e = "vsb-   tuo";
    auto f = "vsb  - \t\n tuo";

    REQUIRE(parser3(c) == c + strlen(c));
    REQUIRE(parser3(d) == d + strlen(d));
    REQUIRE(parser3(e) == e + strlen(e));
    REQUIRE(parser3(f) == f + strlen(f));
}

TEST_CASE("Parse repeat")
{
    auto parser1 = repeat(parse_digit, 3);

    REQUIRE(parser1("") == nullptr);
    REQUIRE(parser1("1") == nullptr);
    REQUIRE(parser1("12") == nullptr);
    REQUIRE(parser1("12x") == nullptr);

    auto a = "123";
    REQUIRE(parser1(a) == a + 3);
}

TEST_CASE("Parse complex")
{
    auto parser1 = any_of(std::vector<Parser>{
        parse_digit,
        create_char_parser('x'),
    });
    auto parser2 = repeat(parse_digit, 2);
    auto parser3 = sequence(std::vector<Parser>{parser1, parser2});

    REQUIRE(parser3("") == nullptr);
    REQUIRE(parser3("y23") == nullptr);
    REQUIRE(parser3("12") == nullptr);
    REQUIRE(parser3("x") == nullptr);

    auto a = "123";
    REQUIRE(parser3(a) == a + 3);

    auto b = "x34";
    REQUIRE(parser3(b) == b + 3);
}

TEST_CASE("Parse regex 1")
{
    auto parser1 = regex_match(R"(\d{2,4})");

    REQUIRE(parser1("") == nullptr);
    REQUIRE(parser1("1") == nullptr);
    REQUIRE(parser1("abcd") == nullptr);
    REQUIRE(parser1("a123") == nullptr);

    auto a = "123";
    REQUIRE(parser1(a) == a + 3);

    auto b = "123ab";
    REQUIRE(parser1(b) == b + 3);

    auto c = "12345";
    REQUIRE(parser1(c) == c + 4);
}

TEST_CASE("Parse regex 2")
{
    auto parser1 = regex_match(R"([a-zA-Z]{3}\d{2,4}$)");

    REQUIRE(parser1("") == nullptr);
    REQUIRE(parser1("abc1") == nullptr);
    REQUIRE(parser1("abc12345") == nullptr);
    REQUIRE(parser1("ab123") == nullptr);
    REQUIRE(parser1("1abc123") == nullptr);

    auto a = "abc1234";
    REQUIRE(parser1(a) == a + 7);

    auto b = "abc12";
    REQUIRE(parser1(b) == b + 5);
}

TEST_CASE("Skip Whitespace")
{
    auto parser1 = skip_ws();

    auto a = "   123";
    REQUIRE(parser1(a) == a + 3);

    auto b = "123";
    REQUIRE(parser1(b) == b);

    auto c = "\n\r\t123";
    REQUIRE(parser1(c) == c + 3);
}
