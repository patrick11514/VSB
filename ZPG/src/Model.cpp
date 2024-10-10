#include "Model.hpp"
#include <algorithm>

Model::Model(float points[])
{
    this->points = new float[sizeof(points)];
}

Model::~Model()
{
    delete this->points;
}
