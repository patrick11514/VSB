#pragma once

#include <optional>
#include <string>
#include <exception>
#include <vector>
#include <unordered_map>

class Value
{
public:
    virtual ~Value();

    virtual Value *operator[](size_t) const;
    virtual Value *operator[](std::string) const;
};

class Integer : public Value
{
private:
    int value;

public:
    Integer(int value);

    int get_value() const;
};

class Null : public Value
{
};

class Array : public Value
{
private:
    std::vector<Value *> value;

public:
    Array(std::vector<Value *> values);

    void append(const Value *&value);

    size_t size() const;
    Value *operator[](size_t index) const final;
};

class Object : public Value
{
private:
    std::unordered_map<std::string, Value *> value;

public:
    Object() = default;
    Object(std::vector<std::pair<std::string, Value *>> values);

    bool insert(std::string key, Value *value);

    size_t size() const;
    std::vector<std::string> keys() const;
};

class UnIndexableException : public std::exception
{
public:
    char *what();
};