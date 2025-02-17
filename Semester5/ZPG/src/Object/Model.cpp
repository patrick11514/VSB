#include "Model.hpp"

#include <GL/glew.h>
#include <algorithm>
#include <cstdio>

Model::Model() : points(nullptr) {}

Model::Model(std::vector<float> points) {
  this->size = points.size();
  this->points = new float[this->size];

  std::copy(points.begin(), points.end(), this->points);
}

Model::Model(const float points[], size_t size) {
  this->size = size;
  this->points = new float[this->size];

  std::copy(points, points + size, this->points);
}

Model::Model(const Model &other) {
  this->size = other.size;
  this->points = new float[this->size];
  std::copy(other.points, other.points + other.size - 1, this->points);
}

Model::Model(Model &&other) {
  std::swap(this->size, other.size);
  std::swap(this->points, other.points);
}

Model::~Model() { delete[] this->points; }

void Model::putDataToBuffer() const {
  glBufferData(GL_ARRAY_BUFFER, this->getPointCount() * sizeof(float),
               this->points, GL_STATIC_DRAW);
}

int Model::getPointCount() const { return this->size; }
