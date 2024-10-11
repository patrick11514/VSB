#pragma once

#include <cstddef>
#include <vector>

using std::size_t;

class Model
{
private:
    float *points;
    size_t size;

public:
    Model(std::vector<float> points);
    ~Model();

    void putDataToBuffer() const;
};