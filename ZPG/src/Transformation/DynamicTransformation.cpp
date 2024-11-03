#include "DynamicTransformation.hpp"

glm::mat4 DynamicTransformation::getMatrix(float elapsedTime) const {
  glm::mat4 mat{1.f};

  for (auto &tran : this->transformations) {
    auto *tranPtr = tran.get();

    if (dynamic_cast<DynamicTransformation *>(tranPtr) != nullptr) {
      mat = static_cast<DynamicTransformation *>(tranPtr)->getMatrix(
                elapsedTime) *
            mat;
    } else {
      mat = tran->getMatrix() * mat;
    }
  }

  return mat;
}
