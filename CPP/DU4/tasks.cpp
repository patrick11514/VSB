#include "tasks.h"

Value *Value::operator[](const size_t) const
{
    throw UnIndexableException();
}

Value *Value::operator[](const std::string &) const
{
    throw UnIndexableException();
}

Value *Value::clone() const
{
    return new Value();
}

void Value::accept(const Visitor &visitor) const
{
    visitor.visitValue(*this);
}

void Value::accept(const MutatingVisitor &visitor)
{
    visitor.visitValue(*this);
}

Integer::Integer(int value) : value(value)
{
}

int Integer::get_value() const
{
    return this->value;
}

Integer *Integer::clone() const
{
    return new Integer(this->value);
}

void Integer::accept(const Visitor &visitor) const
{
    visitor.visitInteger(*this);
}

void Integer::accept(const MutatingVisitor &visitor)
{
    visitor.visitInteger(*this);
}

Array::Array(std::vector<Value *> values) : value(std::move(values))
{
}

Array::~Array()
{
    for (auto data : this->value)
    {
        delete data;
    }
}

void Array::append(Value *value)
{
    this->value.emplace_back(value);
}

void Array::remove(const size_t index)
{
    if (index >= this->size())
    {
        return;
    }

    delete this->operator[](index);
    this->value.erase(this->value.begin() + index);
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

Array *Array::clone() const
{
    std::vector<Value *> copy;
    copy.reserve(this->size());

    for (auto item : this->value)
    {
        copy.emplace_back(item->clone());
    }

    return new Array(std::move(copy));
}

void Array::accept(const Visitor &visitor) const
{
    visitor.visitArray(*this);
}

void Array::accept(const MutatingVisitor &visitor)
{
    visitor.visitArray(*this);
}

// Object::Object(const std::vector<std::pair<std::string, Value *>> &values)
//{
//     this->value.reserve(values.size());
//
//     for (auto value : values)
//     {
//         this->value.emplace(value.first, value.second);
//     }
// }

Object::Object(std::unordered_map<std::string, Value *> map)
{
    this->value = std::move(map);
}

Object::~Object()
{
    for (auto pair : this->value)
    {
        delete pair.second;
    }
}

void Object::insert(const std::string &key, Value *value)
{
    auto itter = this->value.find(key);

    if (itter != this->value.end())
    {
        delete itter->second;
        itter->second = value;
        return;
    }

    this->value.emplace(key, value);
}

void Object::remove(const std::string &key)
{
    auto itter = this->value.find(key);
    if (itter == this->value.end())
    {
        return;
    }

    delete this->operator[](key);
    this->value.erase(itter);
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

    std::sort(vector.begin(), vector.end());
    return vector;
}

Value *Object::operator[](const std::string &key) const
{
    auto itter = this->value.find(key);

    if (itter == this->value.end())
    {
        return nullptr;
    }

    return itter->second;
}

Object *Object::clone() const
{
    std::unordered_map<std::string, Value *> copy;
    copy.reserve(this->size());

    for (auto pair : this->value)
    {
        copy.emplace(pair.first, pair.second->clone());
    }

    return new Object(std::move(copy));
}

void Object::accept(const Visitor &visitor) const
{
    visitor.visitObject(*this);
}

void Object::accept(const MutatingVisitor &visitor)
{
    visitor.visitObject(*this);
}

char *UnIndexableException::what()
{
    return (char *)"This type doesn't support indexing";
}

void Visitor::visitValue(const Value &) const {}

void Visitor::visitInteger(const Integer &) const {}

void Visitor::visitNull(const Null &) const {}

void Visitor::visitArray(const Array &) const {}

void Visitor::visitObject(const Object &) const {}

void MutatingVisitor::visitValue(Value &) const {}

void MutatingVisitor::visitInteger(Integer &) const {}

void MutatingVisitor::visitNull(Null &) const {}

void MutatingVisitor::visitArray(Array &) const {}

void MutatingVisitor::visitObject(Object &) const {}

void RemoveNullVisitor::visitArray(Array &value) const
{
    std::vector<int> toRemove;
    toRemove.reserve(value.size());

    for (size_t i = 0; i < value.size(); ++i)
    {
        auto val = value[i];
        if (dynamic_cast<Null *>(val) != nullptr)
        {
            value.remove(i);
            // decrese to check new element which will be moved to place of removed one
            --i;
        }
        else if (dynamic_cast<Object *>(val) != nullptr)
        {
            val->accept(*this);
        }
        else if (dynamic_cast<Array *>(val) != nullptr)
        {
            val->accept(*this);
        }
    }
}

void RemoveNullVisitor::visitObject(Object &value) const
{
    std::vector<std::string> keys = value.keys();

    for (auto key : keys)
    {
        auto val = value[key];
        if (dynamic_cast<Null *>(val) != nullptr)
        {
            value.remove(key);
        }
        else if (dynamic_cast<Object *>(val) != nullptr)
        {
            val->accept(*this);
        }
        else if (dynamic_cast<Array *>(val) != nullptr)
        {
            val->accept(*this);
        }
    }
}

PrintVisitor::PrintVisitor(std::ostream &stream) : stream(stream)
{
}

void PrintVisitor::visitInteger(const Integer &value) const
{
    this->stream << value.get_value();
}

void PrintVisitor::visitNull(const Null &) const
{
    this->stream << "null";
}

void PrintVisitor::visitArray(const Array &value) const
{
    this->stream << "[";

    for (size_t i = 0; i < value.size(); ++i)
    {
        if (i != 0)
        {
            this->stream << ", ";
        }

        value[i]->accept(*this);
    }

    this->stream << "]";
}

void PrintVisitor::visitObject(const Object &value) const
{
    this->stream << "{";

    auto keys = value.keys();

    for (size_t i = 0; i < keys.size(); ++i)
    {
        auto key = keys[i];

        if (i != 0)
        {
            this->stream << ", ";
        }
        this->stream << key << ": ";
        value[key]->accept(*this);
    }
    this->stream << "}";
}

void Null::accept(const Visitor &visitor) const
{
    visitor.visitNull(*this);
}

void Null::accept(const MutatingVisitor &visitor)
{
    visitor.visitNull(*this);
}
