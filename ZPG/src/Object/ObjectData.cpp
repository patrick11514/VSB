#include "ObjectData.hpp"

ObjectData::ObjectData(Model model, size_t numberOfAttrs,
                       std::function<int()> sliceAttrs)
    : sliceAttrs(sliceAttrs), numberOfAttrs(numberOfAttrs), model(model) {
  glBindVertexArray(VAO);
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
  this->perRow = this->sliceAttrs();
}

ObjectData::ObjectData(const ObjectData &other)
    : ObjectData(other.model, other.numberOfAttrs, other.sliceAttrs) {}

ObjectData::ObjectData(ObjectData &&other)
    : numberOfAttrs(other.numberOfAttrs), VAO(other.VAO), VBO(other.VBO),
      model(std::move(other.model)) {}

int ObjectData::defaultSlice() {
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0);
  return 3;
}

void ObjectData::bindArray() const { glBindVertexArray(this->VAO); }

bool ObjectData::operator==(const ObjectData &other) const {
  return this->VAO == other.VAO;
}

void ObjectData::drawArrays() const {
  glDrawArrays(GL_TRIANGLES, 0, this->model.getPointCount() / this->perRow);
}
