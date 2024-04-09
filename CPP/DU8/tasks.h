#pragma once

#include <exception>
#include <vector>
#include <memory>

class ArrayException : public std::exception
{
public:
    explicit ArrayException(const char *message) : message(message) {}

    const char *what() const noexcept override
    {
        return this->message;
    }

private:
    const char *message;
};

// TODO: implement NumpyArray

template <typename T>
class NumpyArray
{
    T *data;
    size_t size;

public:
    NumpyArray(std::vector<T> data)
    {
        this->size = data.size();
        this->data = new T[this->size];
        std::copy(data.begin(), data.end(), this->data);
    }

    ~NumpyArray()
    {
        delete[] this->data;
    }

    T &operator[](size_t index)
    {
        if (index >= this->size)
        {
            throw ArrayException("Index out of bound");
        }
        return this->data[index];
    }

    const T &operator[](size_t index) const
    {
        if (index >= this->size)
        {
            throw ArrayException("Index out of bound");
        }

        return this->data[index];
    }
};
