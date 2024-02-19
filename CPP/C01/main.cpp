#include <iostream>
#include <string>
#include <cstring>

class MyString
{
    char *buffer = nullptr;
    size_t size = 0;
    size_t reserved = 0;

public:
    MyString() = default;
    MyString(const char *ptr);
    MyString(const MyString &str);
    MyString(MyString &&str);
    ~MyString();

    MyString &operator=(const MyString &str);
    MyString &operator=(MyString &&str);

    char &operator[](const size_t idx);

    MyString &operator+=(const MyString &strt);

    friend std::ostream &operator<<(std::ostream &os, const MyString &str);
};

MyString::MyString(const char *ptr)
{
    size = strlen(ptr);
    reserved = size + 1;
    buffer = new char[reserved];
    std::copy(ptr, ptr + reserved, buffer);
}

MyString::MyString(const MyString &str)
{
    size = str.size;
    reserved = str.reserved;
    buffer = new char[reserved];
    std::copy(str.buffer, str.buffer + reserved, str.buffer);
}

MyString::MyString(MyString &&str)
{
    size = str.size;
    reserved = str.reserved;
    buffer = str.buffer;
    str.buffer = nullptr;
}

MyString &MyString::operator=(const MyString &str)
{
    if (buffer != nullptr)
    {
        delete[] buffer;
    }

    size = str.size;
    reserved = str.reserved;
    buffer = new char[reserved];
    std::copy(str.buffer, str.buffer + reserved, str.buffer);

    return *this;
}

MyString &MyString::operator=(MyString &&str)
{
    if (buffer != nullptr)
    {
        delete[] buffer;
    }

    size = str.size;
    reserved = str.reserved;
    buffer = str.buffer;
    str.buffer = nullptr;

    return *this;
}

MyString::~MyString()
{
    if (buffer != nullptr)
    {
        delete[] buffer;
    }
}

char &MyString::operator[](const size_t idx)
{
    return buffer[idx];
}

MyString &MyString::operator+=(const MyString &str)
{
    const auto newSize = size + str.size;
    if (reserved < newSize + 1)
    {
        reserved = newSize + 1;
        char *newData = new char[reserved];
        std::copy(buffer, buffer + size + 1, newData);
        delete[] buffer;
        buffer = newData;
    }

    std::copy(str.buffer, str.buffer + str.size + 1, buffer + size);
    size = newSize;

    return *this;
}

std::ostream &operator<<(std::ostream &os, const MyString &str)
{
    if (str.buffer == nullptr)
    {
        return os;
    }

    return os << str.buffer;
}

typedef int AircraftId;

int main(int argc, char **argv)
{
    // std::cout << "Hello World" << std::endl;

    // std::flush(std::cout);
    // std::endl(std::cout);

    // std::string str = "Ahoj";

    MyString str{"bbbb"};
    str += MyString{"cccc"};
    std::cout << str << std::endl;

    return 0;
}
