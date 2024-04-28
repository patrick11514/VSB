#pragma once

#include <string>
#include <sstream>

class Record
{
    std::string timestamp;
    unsigned long payload;
    u_int8_t getByte(size_t number) const;

public:
    Record(const std::string &input);

    bool operator>(const Record &other) const;
    bool operator<(const Record &other) const;
    bool operator==(const Record &other) const;
    bool operator!=(const Record &other) const;

    const std::string &getTime() const;
    unsigned int getBattery() const;
    float getTemp() const;
    float getHumidity() const;

    std::string toString() const;
};

namespace std
{
    template <>
    struct hash<Record>
    {
        std::size_t operator()(const Record &record) const
        {
            return std::hash<std::string>{}(record.toString());
        }
    };
};