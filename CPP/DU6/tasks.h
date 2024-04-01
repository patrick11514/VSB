#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <variant>
#include <map>
#include <optional>
#include <iomanip>
#include <cmath>
#include <functional>
#include <bitset>

// JSON document: implement parsing.
// JSON is implemented using algebraic data types here, it should be more practical than working
// with the previous OOP hierarchy and pointers.
struct Boolean
{
    bool value;

    bool operator==(const Boolean &other) const;
};

struct Number
{
    double value;

    bool operator==(const Number &other) const;
};

struct Null
{
    bool operator==(const Null &) const;
};

struct String
{
    std::string value;

    bool operator==(const String &other) const;
};

struct Array;
struct Object;

using Value = std::variant<Boolean, Number, String, Null, Array, Object>;

struct Array
{
    std::vector<Value> items;

    bool operator==(const Array &other) const;
};

struct Object
{
    std::map<std::string, Value> items;

    bool operator==(const Object &other) const;

    [[nodiscard]] std::vector<std::string> keys() const
    {
        std::vector<std::string> keys;
        keys.reserve(this->items.size());
        for (const auto &key : this->items)
        {
            keys.push_back(key.first);
        }
        return keys;
    }
};

template <class... Ts>
struct overloaded : Ts...
{
    using Ts::operator()...;
};
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

/**
 * @brief Skip spaces at the start in input stream
 * @param is InputStream
 */
void skipSpaces(std::istream &is);
/**
 * @brief Check if input stream contains string
 * @param is Input Stream
 * @param str String to be checked
 * @return true if input stream contain string, otherwise false
 */
bool checkString(std::istream &is, const std::string &str);
/**
 * @brief Tries to parse null
 * @param is Input Stream
 * @return Null instance if null is presented, otherwise std::nullopt
 */
std::optional<Null> parseNull(std::istream &is);
std::optional<Boolean> parseBoolean(std::istream &is);
/**
 * @brief Tries to parse string in input stream
 * @param is Input Stream
 * @return String instance, if parsing was success full, std::nullopt, if not
 */
std::optional<String> parseString(std::istream &is);
/**
 * @brief Tries to parse number in input stream
 * @param is Input Stream
 * @return Number instance, if parsing was sucessfull, std::nullopt otherwise
 */
std::optional<Number> parseNumber(std::istream &is);
/**
 * @brief Tries to parse array in input stream
 * @param is Input Stream
 * @return Array instance, if parsing was sucessful, std::nullopt otherwise
 */
std::optional<Array> parseArray(std::istream &is);
/**
 * @brief Tries to parse object in input stream
 * @param is Input Stream
 * @return Object instance, if parsing was sucessful, std::nullopt otherwise
 */
std::optional<Object> parseObject(std::istream &is);

using ParserVector = std::vector<std::function<std::optional<Value>(std::istream &)>>;

// TODO: Implement the following functions
std::optional<Value> parse_json(std::istream &is);
std::ostream &operator<<(std::ostream &os, const Value &value);

/**
 * @brief Encode 8byte data to vector
 * @param data Reference to data
 * @param bytes Bytes to be encoded
 */
void encode8Bytes(std::vector<uint8_t> &data, size_t bytes);

std::vector<uint8_t> serialize(const Value &value);

using VecIt = std::vector<uint8_t>::const_iterator;
size_t decode8Bytes(VecIt &begin, VecIt &end);
std::optional<Value> deserializeAll(VecIt &begin, VecIt &end);

Value deserialize(const std::vector<uint8_t> &data);
