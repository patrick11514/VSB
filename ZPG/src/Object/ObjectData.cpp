#include "ObjectData.hpp"

ObjectData::ObjectData(Model model, size_t numberOfAttrs,
                       std::function<void()> sliceAttrs)
    : model(model), sliceAttrs(sliceAttrs), numberOfAttrs(numberOfAttrs) {
  glGenBuffers(1, &this->VBO); // generate the VBO
  glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
  this->model.putDataToBuffer();

  // Vertex Array Object (VAO)
  glGenVertexArrays(1, &this->VAO); // generate the VAO
  glBindVertexArray(this->VAO);     // bind the VAO

  for (size_t i = 0; i < this->numberOfAttrs; ++i) {
    glEnableVertexAttribArray(i); // enable vertex attributes
  }

  glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
  this->sliceAttrs();
}

ObjectData::ObjectData(const ObjectData &other)
    : ObjectData(other.model, other.numberOfAttrs, other.sliceAttrs) {}

ObjectData::ObjectData(ObjectData &&other)
    : VBO(other.VBO), VAO(other.VAO), model(std::move(other.model)),
      numberOfAttrs(other.numberOfAttrs) {}

void ObjectData::defaultSlice() {
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0);
}

void ObjectData::setArray() const { glBindVertexArray(this->VAO); }

bool ObjectData::operator==(const ObjectData &other) const {
  return this->VAO == other.VAO;
}
