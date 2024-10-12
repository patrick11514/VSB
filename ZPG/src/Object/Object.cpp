#include "Object.hpp"

Object::Object(ObjectData data, const ShaderProgram &shaderProgram,
               Transformations transformations,
               std::function<void(const glm::mat4x4 &transformationMatrix,
                                  const ShaderProgram &shaderProgram)>
                   renderFunction)
    : data(data), shaderProgram(shaderProgram),
      transformations(transformations), renderFunction(renderFunction) {}

void Object::draw() const {
  this->data.setArray();
  this->shaderProgram.setProgram();

  this->renderFunction(this->transformations.getFinalTransfomarmation(), this->shaderProgram);
}
