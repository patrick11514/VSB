// This tells doctest to provide a main() function
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <array>
#include <memory>
#include <utility>
#include <iostream>
#include <optional>
#include <set>
#include <vector>

#include "doctest.h"
#include "tasks.h"

using Stream = std::stringstream;
using JsonParseResult = std::optional<Value>;

// Parse helpers
static void check_fail(JsonParseResult result)
{
    REQUIRE(!result.has_value());
}

static void check_null(JsonParseResult result)
{
    REQUIRE(result.has_value());
    REQUIRE(result == Value{Null{}});
}

static void check_bool(JsonParseResult result, bool value)
{
    REQUIRE(result.has_value());
    REQUIRE(result == Value{Boolean{value}});
}

static void check_number(JsonParseResult result, double value)
{
    REQUIRE(result.has_value());

    Number number = std::get<Number>(result.value());
    REQUIRE(number.value == doctest::Approx(value));
}

static void check_string(JsonParseResult result, std::string value)
{
    REQUIRE(result.has_value());
    REQUIRE(result == Value{String{value}});
}

static void check_remainder(std::istream &is, std::string expected)
{
    std::string value;
    while (true)
    {
        int ch = is.get();
        if (!is.good())
            break;
        value += static_cast<char>(ch);
    }
    REQUIRE(value == expected);
}

template <size_t N>
static void check_array(JsonParseResult result, std::array<Value, N> expected = {})
{
    REQUIRE(result.has_value());
    REQUIRE(std::holds_alternative<Array>(result.value()));

    Array array = std::get<Array>(result.value());
    REQUIRE(array.items.size() == N);
    for (size_t i = 0; i < N; i++)
    {
        REQUIRE(array.items[i] == expected[i]);
    }
}

template <size_t N>
static void check_object(Object &object,
                         std::array<std::pair<std::string, Value>, N> expected = {})
{

    REQUIRE(object.items.size() == N);
    auto keys = object.keys();

    std::set<std::string> keySet;
    std::copy(keys.begin(), keys.end(), std::inserter(keySet, keySet.end()));

    for (const auto &[key, value] : expected)
    {
        auto k_found = keySet.find(key) != keySet.end();
        REQUIRE(k_found);
        auto value_equal = object.items[key] == value;
        REQUIRE(value_equal);
    }
}

template <size_t N>
static void check_object(JsonParseResult result,
                         std::array<std::pair<std::string, Value>, N> expected = {})
{
    REQUIRE(result.has_value());
    REQUIRE(std::holds_alternative<Object>(result.value()));

    Object object = std::get<Object>(result.value());
    check_object(object, expected);
}

// Print helpers
static void check_print(Value value, std::string expected)
{
    std::stringstream ss;
    ss << value;
    REQUIRE(ss.str() == expected);
}

// Quick value constructors
static Value null()
{
    return Null{};
}

static Value boolean(bool value = true)
{
    return Boolean{value};
}

static Value number(double value = 0.0)
{
    return Number{value};
}

static Value string(std::string value = "")
{
    return String{std::move(value)};
}

template <size_t N>
static Value array(std::array<Value, N> expected = {})
{
    Array array{};
    for (auto &item : expected)
    {
        array.items.push_back(std::move(item));
    }
    return array;
}

static std::pair<std::string, Value> kv(std::string key, Value value)
{
    return std::make_pair(key, std::move(value));
}

template <size_t N>
static Value object(std::array<std::pair<std::string, Value>, N> expected = {})
{
    Object object{};
    for (auto &item : expected)
    {
        object.items[item.first] = std::move(item.second);
    }
    return object;
}
/*
TEST_CASE("Parse empty input") {
    Stream input("");
    check_fail(parse_json(input));
}

TEST_CASE("Consume spaces") {
    Stream input("   ");
    check_fail(parse_json(input));
    check_remainder(input, "");
}

TEST_SUITE("Parse null") {
    TEST_CASE("Null") {
        Stream input("null");
        check_null(parse_json(input));
    }

    TEST_CASE("Space in the middle") {
        Stream input("nu ll");
        check_fail(parse_json(input));
    }

    TEST_CASE("Invalid 1") {
        Stream input("nuxll");
        check_fail(parse_json(input));
    }

    TEST_CASE("Invalid 2") {
        Stream input("nu");
        check_fail(parse_json(input));
    }

    TEST_CASE("Content after") {
        Stream input("nullx");
        check_null(parse_json(input));
        check_remainder(input, "x");
    }

    TEST_CASE("Space after") {
        Stream input("null x");
        check_null(parse_json(input));
        check_remainder(input, " x");
    }
}

TEST_SUITE("Parse bool") {
    TEST_CASE("True") {
        Stream input("true");
        check_bool(parse_json(input), true);
    }

    TEST_CASE("False") {
        Stream input("false");
        check_bool(parse_json(input), false);
    }

    TEST_CASE("Skip spaces") {
        Stream input("        false");
        check_bool(parse_json(input), false);
    }

    TEST_CASE("Invalid 1") {
        Stream input("f");
        check_fail(parse_json(input));
    }

    TEST_CASE("Invalid 2") {
        Stream input("fals");
        check_fail(parse_json(input));
    }

    TEST_CASE("Invalid 3") {
        Stream input("t");
        check_fail(parse_json(input));
    }

    TEST_CASE("Invalid 4") {
        Stream input("tru");
        check_fail(parse_json(input));
    }

    TEST_CASE("Invalid 5") {
        Stream input("tr ue");
        check_fail(parse_json(input));
    }

    TEST_CASE("Content after") {
        Stream input("false, foo");
        check_bool(parse_json(input), false);
        check_remainder(input, ", foo");
    }

    TEST_CASE("Space after") {
        Stream input("true foo");
        check_bool(parse_json(input), true);
        check_remainder(input, " foo");
    }
}
TEST_SUITE("Parse string") {
    TEST_CASE("Empty") {
        Stream input(R"("")");
        check_string(parse_json(input), "");
    }

    TEST_CASE("Only first quote") {
        Stream input(R"(")");
        check_fail(parse_json(input));
    }

    TEST_CASE("Spaces before") {
        Stream input(R"(   "foo")");
        check_string(parse_json(input), "foo");
    }

    TEST_CASE("Missing opening quote") {
        Stream input(R"(foo")");
        check_fail(parse_json(input));
    }

    TEST_CASE("Missing closing quote") {
        Stream input(R"("foo)");
        check_fail(parse_json(input));
    }

    TEST_CASE("Escaped backslash") {
        Stream input(R"("x\\y\\z")");
        check_string(parse_json(input), R"(x\y\z)");
    }

    TEST_CASE("Escaped quote") {
        Stream input(R"("x\"y\"z")");
        check_string(parse_json(input), R"(x"y"z)");
    }

    TEST_CASE("Escaped complex") {
        Stream input(R"("\"\\\\\"")");
        check_string(parse_json(input), R"("\\")");
    }

    TEST_CASE("Escaped invalid") {
        Stream input(R"("\\y\x")");
        check_fail(parse_json(input));
    }

    TEST_CASE("Unterminated escape sequence") {
        Stream input(R"("\)");
        check_fail(parse_json(input));
    }

    TEST_CASE("Escaped last quote") {
        Stream input(R"("\")");
        check_fail(parse_json(input));
    }

    TEST_CASE("Single char") {
        Stream input(R"("a")");
        check_string(parse_json(input), "a");
    }

    TEST_CASE("Long string") {
        Stream input(R"("foobarbaz")");
        check_string(parse_json(input), "foobarbaz");
    }

    TEST_CASE("Spaces inside") {
        Stream input(R"(" foo bar     baz ")");
        check_string(parse_json(input), " foo bar     baz ");
    }

    TEST_CASE("Content after") {
        Stream input(R"("foo bar"baz)");
        check_string(parse_json(input), "foo bar");
        check_remainder(input, "baz");
    }

    TEST_CASE("Space after") {
        Stream input(R"("foo bar"   baz)");
        check_string(parse_json(input), "foo bar");
        check_remainder(input, "   baz");
    }
}
TEST_SUITE("Parse number") {
    TEST_CASE("Zero") {
        Stream input("0");
        check_number(parse_json(input), 0);
    }

    TEST_CASE("Zero with decimal point") {
        Stream input("0.");
        check_number(parse_json(input), 0);
    }

    TEST_CASE("Zero with decimal part") {
        Stream input("0.5");
        check_number(parse_json(input), 0.5);
    }

    TEST_CASE("Negative zero") {
        Stream input("-0");
        check_number(parse_json(input), 0);
    }

    TEST_CASE("Spaces before") {
        Stream input("    -1");
        check_number(parse_json(input), -1);
    }

    TEST_CASE("Only dash") {
        Stream input("-");
        check_fail(parse_json(input));
    }

    TEST_CASE("Multiple dashes") {
        Stream input("--1");
        check_fail(parse_json(input));
    }

    TEST_CASE("Point at the beginning") {
        Stream input(".1");
        check_fail(parse_json(input));
    }

    TEST_CASE("Multiple points") {
        Stream input("15..1");
        check_number(parse_json(input), 15);
        check_remainder(input, ".1");
    }

    TEST_CASE("Large integer") {
        Stream input("123456");
        check_number(parse_json(input), 123456);
    }

    TEST_CASE("Negative integer") {
        Stream input("-123456");
        check_number(parse_json(input), -123456);
    }

    TEST_CASE("Small decimal") {
        Stream input("1.5");
        check_number(parse_json(input), 1.5);
    }

    TEST_CASE("Large decimal") {
        Stream input("528479.75");
        check_number(parse_json(input), 528479.75);
    }

    TEST_CASE("Negative decimal") {
        Stream input("-123456.275");
        check_number(parse_json(input), -123456.275);
    }

    TEST_CASE("Content after") {
        Stream input("123x");
        check_number(parse_json(input), 123);
        check_remainder(input, "x");
    }

    TEST_CASE("Space after") {
        Stream input("123 x");
        check_number(parse_json(input), 123);
        check_remainder(input, " x");
    }
}

TEST_SUITE("Parse array") {
    TEST_CASE("Empty") {
        Stream input("[]");
        check_array<0>(parse_json(input));
    }

    TEST_CASE("Empty with spaces") {
        Stream input("[   ]");
        check_array<0>(parse_json(input));
    }

    TEST_CASE("Spaces before") {
        Stream input("   [ ]");
        check_array<0>(parse_json(input));
    }

    TEST_CASE("Only opening brace") {
        Stream input("[");
        check_fail(parse_json(input));
    }

    TEST_CASE("Only closing brace") {
        Stream input("]");
        check_fail(parse_json(input));
    }

    TEST_CASE("Extra comma 1") {
        Stream input("[,]");
        check_fail(parse_json(input));
    }

    TEST_CASE("Extra comma 2") {
        Stream input("[true,]");
        check_fail(parse_json(input));
    }

    TEST_CASE("Extra comma 3") {
        Stream input("[,true]");
        check_fail(parse_json(input));
    }

    TEST_CASE("Extra comma 4") {
        Stream input("[true,,false]");
        check_fail(parse_json(input));
    }

    TEST_CASE("Single null") {
        Stream input("[null]");
        check_array(parse_json(input), std::array{null()});
    }

    TEST_CASE("Single bool") {
        Stream input("[true]");
        check_array(parse_json(input), std::array{boolean(true)});
    }

    TEST_CASE("Single number") {
        Stream input("[123]");
        check_array(parse_json(input), std::array{number(123)});
    }

    TEST_CASE("Single string") {
        Stream input(R"(["foo"])");
        check_array(parse_json(input), std::array{string("foo")});
    }

    TEST_CASE("Invalid item") {
        Stream input(R"([x])");
        check_fail(parse_json(input));
    }

    TEST_CASE("Multiple items") {
        Stream input(R"(["foo",1,true,null,2.5])");
        check_array(parse_json(input), std::array{
                string("foo"),
                number(1),
                boolean(true),
                null(),
                number(2.5)
        });
    }

    TEST_CASE("Multiple items with spaces") {
        Stream input(R"([    "foo"  ,  1 , false,  null, 4])");
        check_array(parse_json(input), std::array{
                string("foo"),
                number(1),
                boolean(false),
                null(),
                number(4)
        });
    }

    TEST_CASE("Missing separator") {
        Stream input(R"([1true])");
        check_fail(parse_json(input));
    }

    TEST_CASE("Nested arrays") {
        Stream input(R"([[1], [], [true, "foo"], null, [[[1], 2], 3]])");
        check_array(parse_json(input), std::array{
                array(std::array{number(1)}),
                array<0>(),
                array(std::array{boolean(true), string("foo")}),
                null(),
                array(std::array{
                        array(std::array{
                                array(std::array{number(1)}),
                                number(2)
                        }),
                        number(3)
                })
        });
    }

    TEST_CASE("Content after") {
        Stream input("[true]x");
        check_array(parse_json(input), std::array{boolean(true)});
        check_remainder(input, "x");
    }

    TEST_CASE("Space after") {
        Stream input("[1] x");
        check_array(parse_json(input), std::array{number(1)});
        check_remainder(input, " x");
    }
}
TEST_SUITE("Parse object") {
    TEST_CASE("Empty") {
        Stream input("{}");
        check_object<0>(parse_json(input));
    }

    TEST_CASE("Empty with spaces") {
        Stream input("{   }");
        check_object<0>(parse_json(input));
    }

    TEST_CASE("Spaces before") {
        Stream input("   { }");
        check_object<0>(parse_json(input));
    }

    TEST_CASE("Only opening brace") {
        Stream input("{");
        check_fail(parse_json(input));
    }

    TEST_CASE("Only closing brace") {
        Stream input("}");
        check_fail(parse_json(input));
    }

    TEST_CASE("Extra comma 1") {
        Stream input("{,}");
        check_fail(parse_json(input));
    }

    TEST_CASE("Extra comma 2") {
        Stream input(R"({"a": true,})");
        check_fail(parse_json(input));
    }

    TEST_CASE("Extra comma 3") {
        Stream input(R"({,"a": true})");
        check_fail(parse_json(input));
    }

    TEST_CASE("Extra comma 4") {
        Stream input(R"({"a": true,, "b": false})");
        check_fail(parse_json(input));
    }

    TEST_CASE("Unterminated quote in key") {
        Stream input(R"({"a:true})");
        check_fail(parse_json(input));
    }

    TEST_CASE("Wrong key type 1") {
        Stream input(R"({1: true})");
        check_fail(parse_json(input));
    }

    TEST_CASE("Wrong key type 2") {
        Stream input(R"({false: true})");
        check_fail(parse_json(input));
    }

    TEST_CASE("Wrong key type 3") {
        Stream input(R"({null: true})");
        check_fail(parse_json(input));
    }

    TEST_CASE("Only key") {
        Stream input(R"({"a"})");
        check_fail(parse_json(input));
    }

    TEST_CASE("Missing colon") {
        Stream input(R"({"a" true})");
        check_fail(parse_json(input));
    }

    TEST_CASE("Double colon") {
        Stream input(R"({"a":: true})");
        check_fail(parse_json(input));
    }

    TEST_CASE("Missing value") {
        Stream input(R"({"a":})");
        check_fail(parse_json(input));
    }

    TEST_CASE("Single bool") {
        Stream input(R"({"a": true})");
        check_object(parse_json(input), std::array{
                kv("a", boolean(true))
        });
    }

    TEST_CASE("Single number") {
        Stream input(R"({"a": 1})");
        check_object(parse_json(input), std::array{
                kv("a", number(1))
        });
    }

    TEST_CASE("Single string") {
        Stream input(R"({"a": "foo"})");
        check_object(parse_json(input), std::array{
                kv("a", string("foo"))
        });
    }

    TEST_CASE("Spaces inside") {
        Stream input(R"({    "a"   :  true  })");
        check_object(parse_json(input), std::array{
                kv("a", boolean(true))
        });
    }

    TEST_CASE("Invalid key") {
        Stream input(R"({x: 1})");
        check_fail(parse_json(input));
    }

    TEST_CASE("Invalid value") {
        Stream input(R"({"x": x})");
        check_fail(parse_json(input));
    }

    TEST_CASE("Multiple items") {
        Stream input(R"({"foo": "bar", "bar": 1, "x": true})");
        check_object(parse_json(input), std::array{
                kv("foo", string("bar")),
                kv("bar", number(1)),
                kv("x", boolean(true))
        });
    }

    TEST_CASE("Nested arrays") {
        Stream input(R"({"x": [1], "y": [], "b": [true, "foo"], "d": null, "z": [[[1], 2], 3]})");
        check_object(parse_json(input), std::array{
                kv("x", array(std::array{number(1)})),
                kv("y", array<0>()),
                kv("b", array(std::array{boolean(true), string("foo")})),
                kv("d", null()),
                kv("z", array(std::array{
                        array(std::array{
                                array(std::array{number(1)}),
                                number(2)
                        }),
                        number(3)
                }))
        });
    }

    TEST_CASE("Nested objects") {
        Stream input(
                R"({"a": {"abc": [1, 2, true], "foo": null}, "b": [], "c": {"x": [true, false]}})");
        check_object(parse_json(input), std::array{
                kv("a", object(std::array{
                        kv("abc", array(std::array{number(1), number(2), boolean(true)})),
                        kv("foo", null())
                })),
                kv("b", array<0>()),
                kv("c", object(std::array{
                        kv("x", array(std::array{boolean(true), boolean(false)}))
                }))
        });
    }

    TEST_CASE("Nested objects in array") {
        Stream input(R"([{"abc": [1, 2, true], "foo": null}, [], {"x": [true, false]}])");
        check_array(parse_json(input), std::array{
                object(std::array{
                        kv("abc", array(std::array{number(1), number(2), boolean(true)})),
                        kv("foo", null())
                }),
                array<0>(),
                object(std::array{
                        kv("x", array(std::array{boolean(true), boolean(false)}))
                })
        });
    }

    TEST_CASE("Content after") {
        Stream input(R"({"x": true}x)");
        check_object(parse_json(input), std::array{kv("x", boolean(true))});
        check_remainder(input, "x");
    }

    TEST_CASE("Space after") {
        Stream input(R"({"x": true} x)");
        check_object(parse_json(input), std::array{kv("x", boolean(true))});
        check_remainder(input, " x");
    }
}
*/
TEST_CASE("Print null")
{
    check_print(null(), "null");
}

TEST_SUITE("Print bool")
{
    TEST_CASE("True")
    {
        check_print(boolean(true), "true");
    }

    TEST_CASE("False")
    {
        check_print(boolean(false), "false");
    }
}
TEST_SUITE("Print number")
{
    TEST_CASE("Zero")
    {
        check_print(number(0), "0");
    }

    TEST_CASE("One")
    {
        check_print(number(1), "1");
    }

    TEST_CASE("Multi-digit")
    {
        check_print(number(123), "123");
    }

    TEST_CASE("Decimal zero")
    {
        check_print(number(123.0), "123");
    }

    TEST_CASE("Decimal non-zero")
    {
        check_print(number(483.25), "483.25");
    }

    TEST_CASE("Negative")
    {
        check_print(number(-100.5), "-100.5");
    }
}
TEST_SUITE("Print string")
{
    TEST_CASE("Empty")
    {
        check_print(string(), R"("")");
    }

    TEST_CASE("Single char")
    {
        check_print(string("a"), R"("a")");
    }

    TEST_CASE("Multiple chars")
    {
        check_print(string("abc def"), R"("abc def")");
    }

    TEST_CASE("Quote")
    {
        check_print(string("f\"x\"y"), R"("f\"x\"y")");
    }

    TEST_CASE("Backslash")
    {
        check_print(string("fx\\y"), R"("fx\\y")");
    }
}
TEST_SUITE("Print array")
{
    TEST_CASE("Empty")
    {
        check_print(array<0>(), "[]");
    }

    TEST_CASE("Bool")
    {
        check_print(array(std::array{boolean(true)}), "[true]");
    }

    TEST_CASE("Number")
    {
        check_print(array(std::array{number(1.5)}), "[1.5]");
    }

    TEST_CASE("String")
    {
        check_print(array(std::array{string("1.5")}), R"(["1.5"])");
    }

    TEST_CASE("Multiple")
    {
        check_print(array(std::array{string("1.5"), number(1.2), boolean(false)}), R"(["1.5", 1.2, false])");
    }

    TEST_CASE("Nested array")
    {
        check_print(array(
                        std::array{
                            number(1.5),
                            array(std::array{
                                string("foo"),
                                string("bar\"")})}),
                    R"([1.5, ["foo", "bar\""]])");
    }

    TEST_CASE("Nested object")
    {
        check_print(array(
                        std::array{
                            number(1.5),
                            object(std::array{
                                kv("a", string("foo")),
                                kv("bx", string("bar\""))})}),
                    R"([1.5, {"a": "foo", "bx": "bar\""}])");
    }
}
TEST_SUITE("Print object")
{
    TEST_CASE("Empty")
    {
        check_print(object<0>(), "{}");
    }

    TEST_CASE("Bool")
    {
        check_print(object(std::array{kv("foo", boolean(true))}), R"({"foo": true})");
    }

    TEST_CASE("Number")
    {
        check_print(object(std::array{kv("foo", number(1.5))}), R"({"foo": 1.5})");
    }

    TEST_CASE("String")
    {
        check_print(object(std::array{kv("foo", string("1.5"))}), R"({"foo": "1.5"})");
    }

    TEST_CASE("Nested array")
    {
        check_print(object(
                        std::array{
                            kv("a", number(1.5)),
                            kv("b", array(std::array{
                                        string("foo"),
                                        string("bar\"")}))}),
                    R"({"a": 1.5, "b": ["foo", "bar\""]})");
    }

    TEST_CASE("Nested object")
    {
        check_print(object(
                        std::array{
                            kv("foo", object(std::array{
                                          kv("a", string("foo")),
                                          kv("b", string("bar\""))})),
                            kv("bar", object(std::array{
                                          kv("c", string("foo")),
                                          kv("d", string("bar\""))})),
                        }),
                    R"({"bar": {"c": "foo", "d": "bar\""}, "foo": {"a": "foo", "b": "bar\""}})");
    }
}
/*
static std::vector<uint8_t> NO_SUBJECTS_DATA = {12, 0, 0, 0, 0, 0, 0, 0, 77, 97, 114, 116, 105, 110,
                                                32, 78, 111, 118, 97, 107, 23, 0, 0, 0, 0, 0, 0, 0,
                                                0, 0, 0, 0};
static std::vector<uint8_t> MULTIPLE_SUBJECTS_DATA = {17, 0, 0, 0, 0, 0, 0, 0, 77, 105, 99, 104, 97,
                                                      108, 32, 75, 111, 108, 111, 109, 97, 122, 110,
                                                      105, 107, 23, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0,
                                                      3, 0, 0, 0, 0, 0, 0, 0, 68, 73, 77, 205, 204,
                                                      76, 63, 4, 0, 0, 0, 0, 0, 0, 0, 65, 80, 80,
                                                      83, 0, 0, 128, 63, 4, 0, 0, 0, 0, 0, 0, 0, 67,
                                                      43, 43, 73, 0, 0, 64, 63};

TEST_SUITE("Serde") {

    TEST_CASE("Null") {
        Null null;
        auto ser = serialize(null);

        REQUIRE_EQ(ser, std::vector<uint8_t> { 0 });

        auto deser = deserialize(ser);

        REQUIRE(std::holds_alternative<Null>(deser));
    }

    TEST_CASE("Boolean") {
        Boolean b1 { false };
        Boolean b2 { true };

        auto ser1 = serialize(b1);
        auto ser2 = serialize(b2);

        REQUIRE_EQ(ser1, std::vector<uint8_t> { 1, 0 });
        REQUIRE_EQ(ser2, std::vector<uint8_t> { 1, 1 });

        auto deser1 = deserialize(ser1);
        auto deser2 = deserialize(ser2);

        REQUIRE_EQ(std::get<Boolean>(deser1).value, false);
        REQUIRE_EQ(std::get<Boolean>(deser2).value, true);
    }

    TEST_CASE("Number") {
        Number num { 69 };
        auto ser = serialize(num);
        auto deser = deserialize(ser);

        REQUIRE_EQ(ser.size(), 9);
        REQUIRE_EQ(ser[0], 2);

        REQUIRE_EQ(std::get<Number>(deser).value, 69.0);
    }

    TEST_CASE("String") {
        const std::string contents = "Romane, ne ze zas budes pindat na Discordu";
        String str { contents };
        auto ser = serialize(str);
        auto deser = deserialize(ser);

        REQUIRE_EQ(ser.size(), 1 + 8 + contents.size());
        REQUIRE_EQ(
                ser,
                std::vector<uint8_t> {
                    3, // type id
                    42, 0, 0, 0, 0, 0, 0, 0,   // Size
                    82, 111, 109, 97, 110, 101, 44, 32, 110, 101, 32, 122, 101, 32, 122, 97, 115,
                    32, 98, 117, 100, 101, 115, 32, 112, 105, 110, 100, 97, 116, 32, 110, 97, 32,
                    68, 105, 115, 99, 111, 114, 100, 117 // Contents
                });

        REQUIRE_EQ(std::get<String>(deser).value, contents);
    }

    TEST_CASE("Array") {
        std::vector<std::string> contents {
            "I did my time",
            "And I want out",
            "So effusive",
            "Fade, it doesn't cut",
            "The soul is not so vibrant",
            "The reckoning",
            "The sickening",
            "Packaging subversion",
            "Pseudosacrosanct perversion",
        };

        std::vector<Value> values;

        std::transform(contents.begin(), contents.end(), std::back_inserter(values),
                [](std::string str) -> Value { return String { std::move(str) }; });

        Array arr { values };

        auto ser = serialize(arr);
        REQUIRE_EQ(ser[0], 4);
        REQUIRE_EQ(ser[1], 9);
        REQUIRE_EQ(ser[2], 0);
        REQUIRE_EQ(ser[3], 0);
        REQUIRE_EQ(ser[4], 0);
        REQUIRE_EQ(ser[5], 0);
        REQUIRE_EQ(ser[6], 0);
        REQUIRE_EQ(ser[7], 0);
        REQUIRE_EQ(ser[8], 0);

        auto deser = deserialize(ser);
        REQUIRE(std::holds_alternative<Array>(deser));
        auto& array = std::get<Array>(deser);

        REQUIRE_EQ(array.items.size(), 9);

        std::vector<std::string> deserContents;

        std::transform(array.items.begin(), array.items.end(), std::back_inserter(deserContents),
                [](Value& val) -> std::string { return std::get<String>(val).value; });

        REQUIRE_EQ(deserContents, contents);
    }

    TEST_CASE("Object") {
        std::array values {
            kv("yes", Boolean { true }),
            kv("no", Boolean { false }),
            kv("null", Null{}),
            kv("number", Number { 666 }),
            kv("string", String { "Inside my shell I wait and bleed" })
        };

        auto o = object(values);

        auto serialized = serialize(o);

        REQUIRE_EQ(serialized[0], 5);
        REQUIRE_EQ(serialized[1], 5);
        REQUIRE_EQ(serialized[2], 0);
        REQUIRE_EQ(serialized[3], 0);
        REQUIRE_EQ(serialized[4], 0);
        REQUIRE_EQ(serialized[5], 0);
        REQUIRE_EQ(serialized[6], 0);
        REQUIRE_EQ(serialized[7], 0);
        REQUIRE_EQ(serialized[8], 0);

        std::vector<uint8_t> bytes {
            0x5, 0x5, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x2, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
            0x6e, 0x6f, 0x1, 0x0, 0x4, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x6e, 0x75, 0x6c, 0x6c,
            0x0, 0x6, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x6e, 0x75, 0x6d, 0x62, 0x65, 0x72, 0x2,
            0x0, 0x0, 0x0, 0x0, 0x0, 0xd0, 0x84, 0x40, 0x6, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x73,
            0x74, 0x72, 0x69, 0x6e, 0x67, 0x3, 0x20, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x49, 0x6e,
            0x73, 0x69, 0x64, 0x65, 0x20, 0x6d, 0x79, 0x20, 0x73, 0x68, 0x65, 0x6c, 0x6c, 0x20, 0x49,
            0x20, 0x77, 0x61, 0x69, 0x74, 0x20, 0x61, 0x6e, 0x64, 0x20, 0x62, 0x6c, 0x65, 0x65, 0x64,
            0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x79, 0x65, 0x73, 0x1, 0x1
        };

        REQUIRE_EQ(bytes, serialized);

        auto deserialized = deserialize(serialized);
        check_object(deserialized, values);
    }

    TEST_CASE("Complex object") {
        std::array values {
            kv("foo", object(std::array{
                kv("a", string("foo")),
                kv("b", string("bar\""))
            })),
            kv("bar", object(std::array{
                kv("c", string("foo")),
                kv("d", string("bar\""))
            })),
            kv("arr", array(std::array{
                    null(),
                    string("If you're"),
                    number(555),
                    string("then I'm"),
                    number(666),
                    string("what's it like to be a heretic"),
                    boolean()
            })),
        };

        auto o = object(values);
        auto serialized = serialize(o);

        std::vector<uint8_t> bytes {
            0x5, 0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
            0x61, 0x72, 0x72, 0x4, 0x7, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3, 0x9, 0x0, 0x0,
            0x0, 0x0, 0x0, 0x0, 0x0, 0x49, 0x66, 0x20, 0x79, 0x6f, 0x75, 0x27, 0x72, 0x65, 0x2, 0x0,
            0x0, 0x0, 0x0, 0x0, 0x58, 0x81, 0x40, 0x3, 0x8, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x74,
            0x68, 0x65, 0x6e, 0x20, 0x49, 0x27, 0x6d, 0x2, 0x0, 0x0, 0x0, 0x0, 0x0, 0xd0, 0x84, 0x40,
            0x3, 0x1e, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x77, 0x68, 0x61, 0x74, 0x27, 0x73, 0x20,
            0x69, 0x74, 0x20, 0x6c, 0x69, 0x6b, 0x65, 0x20, 0x74, 0x6f, 0x20, 0x62, 0x65, 0x20, 0x61,
            0x20, 0x68, 0x65, 0x72, 0x65, 0x74, 0x69, 0x63, 0x1, 0x1, 0x3, 0x0, 0x0, 0x0, 0x0, 0x0,
            0x0, 0x0, 0x62, 0x61, 0x72, 0x5, 0x2, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x0, 0x0,
            0x0, 0x0, 0x0, 0x0, 0x0, 0x63, 0x3, 0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x66, 0x6f,
            0x6f, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x64, 0x3, 0x4, 0x0, 0x0, 0x0, 0x0, 0x0,
            0x0, 0x0, 0x62, 0x61, 0x72, 0x22, 0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x66, 0x6f,
            0x6f, 0x5, 0x2, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
            0x0, 0x61, 0x3, 0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x66, 0x6f, 0x6f, 0x1, 0x0,
            0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x62, 0x3, 0x4, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x62,
            0x61, 0x72, 0x22
        };

        REQUIRE_EQ(serialized, bytes);

        auto deserialized = deserialize(serialized);
        check_object(deserialized, values);
    }

}
*/