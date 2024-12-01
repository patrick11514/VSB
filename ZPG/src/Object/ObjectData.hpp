#pragma once

#include <GL/glew.h>
#include <functional>
#include <stdio.h>
#include <stdlib.h>

#include "Model.hpp"

/**
 * @brief Slice raw float data into specific parts like: point coordinates,
 * normal, color etc...
 */
class ObjectData {
private:
  std::function<int()> sliceAttrs; ///< Function, which will slice attributes
  size_t numberOfAttrs;            ///< Number of attributes
  int perRow;

protected:
  GLuint VAO = 0; ///< Id of VAO on GPU
  GLuint VBO = 0; ///< Id of VBO on GPU
  Model model;    ///< Model sliced by attributes
public:
  /**
   * @brief Default constructor, which will slice Model data into numberOfAttrs
   * attributes using sliceAttrs Function
   * @param model Model which will be sliced
   * @param numberOfAttrs number of attributes after slicing. Defaults to 1 if
   * not specified
   * @param sliceAttrs Lambda function, which should contain several
   * glVertexAttribPointer function calls for define slicing the array. Defaults
   * to default static function ObjectData::defaultSlice
   */
  ObjectData(Model model, size_t numberOfAttrs = 1,
             std::function<int()> sliceAttrs = ObjectData::defaultSlice);
  ObjectData(const ObjectData &other); ///< Copy constructor
  ObjectData(ObjectData &&other);      ///< Move constructor
  virtual ~ObjectData() = default;

  static int defaultSlice(); ///< Default slice function, which will slice the
                             ///< array by 3 floats (x,y,z)

  void bindArray() const; ///< Binds the array on GPU

  // operators
  bool operator==(const ObjectData &other) const; ///< compare operator

  virtual void drawArrays() const;
};
