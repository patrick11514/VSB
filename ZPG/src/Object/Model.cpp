#include "Model.hpp"

#include <GL/glew.h>
#include <algorithm>
#include <cstdio>

Model::Model(std::vector<float> points)
{
    this->size = points.size();
    this->points = new float[this->size];

    std::copy(points.begin(), points.end(), this->points);
}

Model::~Model()
{
    delete this->points;
}

void Model::putDataToBuffer() const
{
    glBufferData(GL_ARRAY_BUFFER, this->size * sizeof(float), this->points, GL_STATIC_DRAW);
}
