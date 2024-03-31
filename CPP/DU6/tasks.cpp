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

std::optional<Value> parse_json(std::istream &is)
{
    return std::optional<Value>();
}

std::ostream &operator<<(std::ostream &os, const Value &value)
{
    // https://en.cppreference.com/w/cpp/utility/variant/visit
    std::visit(overloaded{[&os](auto arg)
                          {
                              os << "other";
                          },
                          [&os](Null val)
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

std::vector<uint8_t> serialize(const Value &value)
{
    return {};
}

Value deserialize(const std::vector<uint8_t> &data)
{
    return Null();
}
