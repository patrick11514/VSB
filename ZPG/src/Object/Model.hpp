#pragma once

#include <cstddef>
#include <vector>

using std::size_t;

/**
 * @brief Class for holding raw float data of specific size
 */
class Model {
private:
  float *points = nullptr;
  size_t size;

public:
  Model();                          ///< Raw constructor
  Model(std::vector<float> points); ///< Constructor from vector of points
  Model(const float points[],
        size_t size);        ///< constructor from float array and size of data
  Model(const Model &other); ///< copy constructor
  Model(Model &&other);      ///< move constructor
  ~Model();                  ///< destructor

  void putDataToBuffer() const; ///< put data to gpu
};
