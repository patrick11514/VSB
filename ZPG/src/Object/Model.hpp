#pragma once

#include <cstddef>
#include <vector>

using std::size_t;

class Model
{
private:
    float *points;

public:
    size_t size;
    Model();
    Model(std::vector<float> points);
    Model(const float points[], size_t size);
    Model(const Model &other);
    Model(Model &&other);
    ~Model();

    void putDataToBuffer() const;
};