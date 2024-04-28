#pragma once

#include <set>
#include <optional>

#include "Record.hpp"

class Collection
{
    std::set<Record> data;

public:
    size_t inputCount = 0;
    Collection(const std::string &fileName);

    void toFile(const std::string &fileName) const;
    std::optional<Record> findRecord(const std::string &timestamp) const;

    void print() const;
    size_t getCurrentCount() const;

    float getMinTemp() const;
    float getMaxTemp() const;
    float getMinHumidity() const;
    float getMaxHumidity() const;

    void printTempHistorgram(int width = 100) const;
    void printHumidityHistorgram(int width = 100) const;
};