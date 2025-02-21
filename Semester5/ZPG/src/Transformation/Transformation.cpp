#include "Transformation.hpp"

std::shared_ptr<Transformation>
Transformation::addTransformation(Transformation *tran) {
  this->transformations.emplace_back(tran);

  return shared_from_this();
}

glm::mat4 Transformation::getMatrix() {
  glm::mat4 mat{1.f};

  for (auto &tran : this->transformations) {
    mat = tran->getMatrix() * mat;
  }

  return mat;
}
