#include "Transformations.hpp"

Transformations* Transformations::addTransformation(const Transformation& tran) {
    this->finalTransformation *= tran.getMatrix();
    return this;
}

glm::mat4x4 Transformations::getFinalTransfomarmation() const {
    return this->finalTransformation;
}
