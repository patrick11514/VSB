#pragma once

/*
 * File: IndexedObjectData.hpp
 * Author: Patrik Mintěl (MIN0150)
 * Description: File contains IndextObjectData class
 */

#include <GL/glew.h>
#include <functional>
#include <stdio.h>
#include <stdlib.h>

#include "Model.hpp"
#include "ObjectData.hpp"

/**
 * @brief Slice raw float data into specific parts like: point coordinates,
 * normal, color etc... and store coodinates's indexes in index buffer
 */
class IndexedObjectData : public ObjectData {
private:
  GLuint IBO;
  std::vector<unsigned int> indicies;

public:
  /**
   * @brief Default constructor, which will slice Model data into numberOfAttrs
   * attributes using sliceAttrs Function
   * @param model Model which will be sliced
   * @param indicies Vector with indicies
   * @param numberOfAttrs number of attributes after slicing. Defaults to 1 if
   * not specified
   * @param sliceAttrs Lambda function, which should contain several
   * glVertexAttribPointer function calls for define slicing the array. Defaults
   * to default static function ObjectData::defaultSlice
   */
  IndexedObjectData(Model model, const std::vector<unsigned int> &indicies,
                    size_t numberOfAttrs = 1,
                    std::function<int()> sliceAttrs = ObjectData::defaultSlice);

  void bindArray() const; ///< Binds the array on GPU

  void drawArrays() const override;
};
