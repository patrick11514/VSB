#include "Transformations.hpp"

void Transformations::addTransformation(const Transformation& tran) {
    this->finalTransformation *= tran.getMatrix();
}

glm::mat4x4 Transformations::getFinalTransfomarmation() const {
    return this->finalTransformation;
}
