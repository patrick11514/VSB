#include "IndexedObjectData.hpp"

IndexedObjectData::IndexedObjectData(Model model,
                                     const std::vector<unsigned int> &indicies,
                                     size_t numberOfAttrs,
                                     std::function<int()> sliceAttrs)
    : ObjectData(model, numberOfAttrs, sliceAttrs), indicies(indicies) {
  ObjectData::bindArray();

  glGenBuffers(1, &this->IBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indicies.size(),
               indicies.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void IndexedObjectData::drawArrays() const {
  glDrawElements(GL_TRIANGLES, this->indicies.size(), GL_UNSIGNED_INT, NULL);
}
