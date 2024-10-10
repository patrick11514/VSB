#pragma once

class Model
{
private:
    float *points;

public:
    Model(float points[]);
    ~Model();
};