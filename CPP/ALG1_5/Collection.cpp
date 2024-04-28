#include <fstream>
#include <iostream>
#include <cstring>
#include <limits>
#include <iomanip>

#include "Collection.hpp"

Collection::Collection(const std::string &fileName)
{
    std::ifstream file(fileName);
    if (!file.is_open())
    {
        throw std::runtime_error("Cannot open file");
    }

    while (file)
    {
        char data[255];
        file.getline(data, sizeof(data));
        if (std::strlen(data) == 0)
        {
            continue;
        }
        ++this->inputCount;
        this->data.emplace(data);
    }

    file.close();
}

void Collection::toFile(const std::string &fileName) const
{
    std::ofstream file(fileName);

    for (auto &item : this->data)
    {
        file << item.toString() << "\n";
    }

    file.close();
}

std::optional<Record> Collection::findRecord(const std::string &timestamp) const
{
    auto it = this->data.find(timestamp);
    if (it == this->data.end())
    {
        return std::nullopt;
    }

    return *it;
}

void Collection::print() const
{
    for (auto &rec : this->data)
    {
        std::cout << rec.getTime() << ": " << std::endl;
        std::cout << "Battery: " << rec.getBattery() << "mV" << std::endl;
        std::cout << "Temp: " << rec.getTemp() << "°C" << std::endl;
        std::cout << "Humidity: " << rec.getHumidity() << "%" << std::endl;
    }
}

size_t Collection::getCurrentCount() const
{
    return this->data.size();
}

float Collection::getMinTemp() const
{
    float temp = std::numeric_limits<float>::max();
    for (auto &rec : this->data)
    {
        auto cTemp = rec.getTemp();
        if (cTemp < temp)
        {
            temp = cTemp;
        }
    }
    return temp;
}

float Collection::getMaxTemp() const
{
    float temp = std::numeric_limits<float>::min();
    for (auto &rec : this->data)
    {
        auto cTemp = rec.getTemp();
        if (cTemp > temp)
        {
            temp = cTemp;
        }
    }
    return temp;
}

float Collection::getMinHumidity() const
{
    float humidity = std::numeric_limits<float>::max();
    for (auto &rec : this->data)
    {
        auto cTemp = rec.getHumidity();
        if (cTemp < humidity)
        {
            humidity = cTemp;
        }
    }
    return humidity;
}

float Collection::getMaxHumidity() const
{
    float humidity = std::numeric_limits<float>::min();
    for (auto &rec : this->data)
    {
        auto cTemp = rec.getHumidity();
        if (cTemp > humidity)
        {
            humidity = cTemp;
        }
    }
    return humidity;
}

void Collection::printTempHistorgram(int width) const
{
    auto max = this->getMaxTemp() + this->getMinTemp();

    for (auto &rec : this->data)
    {
        auto temp = rec.getTemp();
        int count = width * (temp / max);
        std::cout << rec.getTime() << " | " << std::setw(4) << temp << " | ";
        for (int i = 0; i < count; ++i)
        {
            std::cout << "#";
        }

        std::endl(std::cout);
    }
}

void Collection::printHumidityHistorgram(int width) const
{
    auto max = this->getMaxHumidity() + this->getMinHumidity();

    for (auto &rec : this->data)
    {
        auto temp = rec.getHumidity();
        int count = width * (temp / max);
        std::cout << rec.getTime() << " | " << std::setw(4) << temp << " | ";
        for (int i = 0; i < count; ++i)
        {
            std::cout << "#";
        }

        std::endl(std::cout);
    }
}
