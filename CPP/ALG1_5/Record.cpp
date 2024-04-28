#include <format>
#include <sstream>
#include <iostream>

#include "Record.hpp"

u_int8_t Record::getByte(size_t number) const
{
    return ((this->payload >> (4 - number) * 8) & 0b11111111);
}

Record::Record(const std::string &input)
{
    std::stringstream ss;
    ss << input;

    ss >> this->timestamp;
    std::string payload;
    ss >> payload;

    this->payload = std::stoul(payload, nullptr, 16);
}

bool Record::operator>(const Record &other) const
{
    return this->timestamp > other.timestamp;
}

bool Record::operator<(const Record &other) const
{
    return this->timestamp < other.timestamp;
}

bool Record::operator==(const Record &other) const
{
    return this->timestamp == other.timestamp;
}

bool Record::operator!=(const Record &other) const
{
    return !this->operator==(other);
}

const std::string &Record::getTime() const
{
    return this->timestamp;
}

unsigned int Record::getBattery() const
{
    return static_cast<unsigned int>(this->getByte(0)) * 30;
}

float Record::getTemp() const
{
    return (static_cast<float>(1) / 10) * (256 * this->getByte(1) + this->getByte(2));
}

float Record::getHumidity() const
{
    return (static_cast<float>(1) / 10) * (256 * this->getByte(3) + this->getByte(4));
}

std::string Record::toString() const
{
    std::stringstream ss;
    ss << std::uppercase << this->timestamp << " " << std::hex << this->payload;
    return ss.str();
}
