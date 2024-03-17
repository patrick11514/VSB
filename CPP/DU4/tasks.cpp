#include "tasks.h"

Value *Value::operator[](size_t) const
{
    throw new UnIndexableException();
}

Value *Value::operator[](std::string) const
{
    throw new UnIndexableException();
}

Integer::Integer(int value) : value(value)
{
}

int Integer::get_value() const
{
    return this->value;
}

Array::Array(std::vector<Value *> values) : value(std::move(values))
{
}

void Array::append(const Value *&value)
{
    this->value.emplace_back(value);
}

size_t Array::size() const
{
    return this->value.size();
}

Value *Array::operator[](size_t index) const
{
    if (index >= this->value.size())
    {
        return nullptr;
    }

    return this->value[index];
}

Object::Object(std::vector<std::pair<std::string, Value *>> values)
{
    this->value.reserve(values.size());

    for (auto value : values)
    {
        this->value.emplace(value.first, value.second);
    }
}

bool Object::insert(std::string key, Value *value)
{
    auto itter = this->value.find(key);

    if (itter != this->value.end())
    {
        return false;
    }

    this->value.emplace(key, value);
    return true;
}

size_t Object::size() const
{
    return this->value.size();
}

std::vector<std::string> Object::keys() const
{
    std::vector<std::string> vector;
    vector.reserve(this->value.size());

    for (auto pair : this->value)
    {
        vector.emplace_back(pair.first);
    }

    return vector;
}

char *UnIndexableException::what()
{
    return (char *)"This type doesn't support indexing";
}
