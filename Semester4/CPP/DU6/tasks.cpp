#include "tasks.h"

bool Array::operator==(const Array &other) const
{
    return other.items == this->items;
}

bool Object::operator==(const Object &other) const
{
    return other.items == this->items;
}

bool String::operator==(const String &other) const
{
    return other.value == this->value;
}

bool Null::operator==(const Null &) const
{
    return true;
}

bool Number::operator==(const Number &other) const
{
    return other.value == this->value;
}

bool Boolean::operator==(const Boolean &other) const
{
    return other.value == this->value;
}

void skipSpaces(std::istream &is)
{
    while (is)
    {
        char c = is.peek();
        if (std::isspace(c))
        {
            is.get();
            continue;
        }
        return;
    }
}

bool checkString(std::istream &is, const std::string &str)
{
    char *c = new char[str.size()];
    std::fill(c, c + str.size(), 0);

    is.read(c, str.size());
    std::string_view sw(c, str.size());

    if (sw == str)
    {
        delete[] c;
        return true;
    }

    delete[] c;
    return false;
}

std::optional<Null> parseNull(std::istream &is)
{
    char c = is.peek();
    if (c != 'n')
    {
        return std::nullopt;
    }

    if (checkString(is, "null"))
    {
        return Null{};
    }
    return std::nullopt;
}

std::optional<Boolean> parseBoolean(std::istream &is)
{
    char c = is.peek();

    if (c != 't' && c != 'f')
    {
        return std::nullopt;
    }

    if (c == 't' && checkString(is, "true"))
    {
        return Boolean{true};
    }
    else if (c == 'f' && checkString(is, "false"))
    {
        return Boolean{false};
    }
    return std::nullopt;
}

std::optional<String> parseString(std::istream &is)
{
    std::string s;
    bool ended = false;
    bool escape = false;

    char begin = is.peek();
    if (begin == '"')
    {
        is.get();
    }
    else
    {
        return std::nullopt;
    }

    while (is)
    {
        char c = is.peek();

        if (c == '\\' && escape == false)
        {
            escape = true;
            is.get();
            continue;
        }

        if (c == '"' && escape == false)
        {
            ended = true;
            is.get();
            break;
        }

        if (escape && c != '"' && c != '\\')
        {
            return std::nullopt;
        }

        if (escape)
        {
            escape = false;
        }

        is.get();
        s += c;
    }

    if (ended)
    {
        return String{s};
    }
    return std::nullopt;
}

std::optional<Number> parseNumber(std::istream &is)
{
    double number = 0;
    bool negative = false;
    bool ok = false;
    bool afterDecimal = false;
    int decimal = 1;

    char c = is.peek();
    if (!std::isdigit(c) && c != '-')
    {
        return std::nullopt;
    }

    if (c == '-')
    {
        negative = true;
        is.get();
    }

    while (is)
    {
        char c = is.peek();

        if (c == '.')
        {
            if (ok == false)
            {
                return std::nullopt;
            }

            if (afterDecimal)
            {
                break;
            }

            afterDecimal = true;
            is.get();
            continue;
        }

        if (!std::isdigit(c))
        {
            if (ok == false)
            {
                return std::nullopt;
            }

            break;
        }

        if (ok == false)
        {
            ok = true;
        }

        if (!afterDecimal)
        {
            number *= 10;
            number += (c - '0');
        }
        else
        {
            number += static_cast<float>(c - '0') / static_cast<float>(std::pow(10, decimal));
            decimal++;
        }

        is.get();
    }

    if (ok)
    {
        if (negative)
        {
            return Number{(number * -1)};
        }
        return Number{number};
    }

    return std::nullopt;
}

std::optional<Array> parseArray(std::istream &is)
{
    char c = is.peek();
    if (c != '[')
    {
        return std::nullopt;
    }

    is.get();

    std::vector<Value> values;

    bool isComma = false;

    while (is)
    {
        skipSpaces(is);

        char c = is.peek();

        if (c == ']')
        {
            if (isComma)
            {
                return std::nullopt;
            }
            is.get();
            break;
        }

        if (isComma)
        {
            isComma = false;
        }
        else
        {
            if (values.size() > 0)
            {
                return std::nullopt;
            }
        }

        auto result = parse_json(is);

        if (result == std::nullopt)
        {
            return std::nullopt;
        }

        values.emplace_back(result.value());

        skipSpaces(is);

        c = is.peek();

        if (c == ',')
        {
            isComma = true;
            is.get();
            continue;
        }
    }

    return Array{values};
}

std::optional<Object> parseObject(std::istream &is)
{
    char c = is.peek();
    if (c != '{')
    {
        return std::nullopt;
    }

    is.get();

    std::map<std::string, Value> object;

    bool isComma = false;

    while (is)
    {
        skipSpaces(is);

        char c = is.peek();

        if (c == '}')
        {
            if (isComma)
            {
                return std::nullopt;
            }
            is.get();
            break;
        }

        if (isComma)
        {
            isComma = false;
        }
        else
        {
            if (object.size() > 0)
            {
                return std::nullopt;
            }
        }

        auto key = parseString(is);
        if (key == std::nullopt)
        {
            return std::nullopt;
        }

        skipSpaces(is);

        c = is.peek();

        if (c != ':')
        {
            return std::nullopt;
        }

        is.get();

        auto value = parse_json(is);

        if (value == std::nullopt)
        {
            return std::nullopt;
        }

        object.emplace(key.value().value, value.value());

        skipSpaces(is);

        c = is.peek();

        if (c == ',')
        {
            isComma = true;
            is.get();
            continue;
        }
    }

    return Object{object};
}

std::optional<Value> parse_json(std::istream &is)
{
    skipSpaces(is);

    ParserVector parsers = {parseNull, parseBoolean, parseString, parseNumber, parseArray, parseObject};

    for (auto parser : parsers)
    {
        std::optional<Value> val = parser(is);
        if (val != std::nullopt)
        {
            return val;
        }
    }

    return std::nullopt;
}

std::ostream &operator<<(std::ostream &os, const Value &value)
{
    // https://en.cppreference.com/w/cpp/utility/variant/visit
    std::visit(overloaded{[&os](Null)
                          {
                              os << "null";
                          },
                          [&os](Boolean val)
                          {
                              if (val.value == true)
                              {
                                  os << "true";
                              }
                              else
                              {
                                  os << "false";
                              }
                          },
                          [&os](Number val)
                          {
                              os << val.value;
                          },
                          [&os](String val)
                          {
                              os << std::quoted(val.value);
                          },
                          [&os](Array val)
                          {
                              os << "[";
                              for (auto it = val.items.begin(); it != val.items.end(); ++it)
                              {
                                  if (it != val.items.begin())
                                  {
                                      os << ", ";
                                  }
                                  os << *it;
                              }
                              os << "]";
                          },
                          [&os](Object val)
                          {
                              os << "{";
                              for (auto it = val.items.begin(); it != val.items.end(); ++it)
                              {
                                  if (it != val.items.begin())
                                  {
                                      os << ", ";
                                  }
                                  os << std::quoted(it->first) << ": " << it->second;
                              }
                              os << "}";
                          }},

               value);

    return os;
}

void encode8Bytes(std::vector<uint8_t> &data, size_t bytes)
{
    for (int i = 0; i < 8; ++i)
    {
        data.emplace_back((bytes >> (8 * i)) & 0b11111111);
    }
}

std::vector<uint8_t> serialize(const Value &value)
{
    std::vector<uint8_t> data;

    std::visit(overloaded{[&data](Null)
                          {
                              data.emplace_back(0);
                          },
                          [&data](Boolean val)
                          {
                              data.emplace_back(1);
                              if (val.value == true)
                              {
                                  data.emplace_back(1);
                              }
                              else
                              {
                                  data.emplace_back(0);
                              }
                          },
                          [&data](Number val)
                          {
                              data.emplace_back(2);
                              encode8Bytes(data, *((size_t *)&val.value));
                          },
                          [&data](String val)
                          {
                              const std::string &str = val.value;
                              data.emplace_back(3);
                              // encode size
                              encode8Bytes(data, str.size());
                              // encode string
                              for (auto it = str.begin(); it != str.end(); ++it)
                              {
                                  data.emplace_back(*it);
                              }
                          },
                          [&data](Array val)
                          {
                              data.emplace_back(4);

                              const auto &values = val.items;
                              encode8Bytes(data, values.size());

                              for (auto it = values.begin(); it != values.end(); ++it)
                              {
                                  std::vector<uint8_t> valueData = serialize(*it);
                                  data.insert(data.end(), valueData.begin(), valueData.end());
                              }
                          },
                          [&data](Object val)
                          {
                              data.emplace_back(5);
                              const auto &values = val.items;
                              encode8Bytes(data, values.size());

                              for (auto it = values.begin(); it != values.end(); ++it)
                              {
                                  encode8Bytes(data, it->first.size());
                                  for (auto it2 = it->first.begin(); it2 != it->first.end(); ++it2)
                                  {
                                      data.emplace_back(*it2);
                                  }
                                  std::vector<uint8_t> valueData = serialize(it->second);
                                  data.insert(data.end(), valueData.begin(), valueData.end());
                              }
                          }},

               value);

    return data;
}

size_t decode8Bytes(VecIt &begin, VecIt &end)
{
    size_t value = 0;
    for (int i = 0; i < 8; ++i)
    {
        if (begin == end)
        {
            return 0;
        }

        long current = *begin;
        value |= (current << (8 * i));

        ++begin;
    }
    return value;
}

std::optional<Value> deserializeAll(VecIt &begin, VecIt &end)
{
    uint8_t type = *begin;
    ++begin;

    if (begin == end)
    {
        return std::nullopt;
    }

    if (type == 0)
    {
        return Null{};
    }
    else if (type == 1)
    {
        auto val = Boolean{*begin == 1};
        ++begin;
        return val;
    }
    else if (type == 2)
    {
        size_t value = decode8Bytes(begin, end);
        return Number{*((double *)&value)};
    }
    else if (type == 3)
    {
        size_t size = decode8Bytes(begin, end);

        std::string str;
        str.reserve(size);

        for (size_t i = 0; i < size; ++i)
        {
            if (begin == end)
            {
                return std::nullopt;
            }

            str += *begin;
            ++begin;
        }

        return String{str};
    }
    else if (type == 4)
    {
        size_t size = decode8Bytes(begin, end);
        std::vector<Value> values;
        values.reserve(size);

        for (size_t i = 0; i < size; ++i)
        {
            if (begin == end)
            {
                return std::nullopt;
            }

            auto value = deserializeAll(begin, end);

            if (value == std::nullopt)
            {
                return std::nullopt;
            }
            values.emplace_back(value.value());
        }

        return Array{values};
    }
    else if (type == 5)
    {
        size_t size = decode8Bytes(begin, end);
        std::map<std::string, Value> object;

        for (size_t i = 0; i < size; ++i)
        {
            if (begin == end)
            {
                return std::nullopt;
            }

            // key
            size_t keySize = decode8Bytes(begin, end);
            std::string key;
            key.reserve(keySize);

            for (size_t i = 0; i < keySize; ++i)
            {
                if (begin == end)
                {
                    return std::nullopt;
                }

                key += *begin;
                ++begin;
            }

            auto value = deserializeAll(begin, end);

            if (value == std::nullopt)
            {
                return std::nullopt;
            }

            object.emplace(key, value.value());
        }

        return Object{object};
    }

    return std::nullopt;
}

Value deserialize(const std::vector<uint8_t> &data)
{
    auto begin = data.begin();
    auto end = data.end();

    auto result = deserializeAll(begin, end);

    if (result.has_value())
    {
        return result.value();
    }

    return Null{};
}
