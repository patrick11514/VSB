#include "Object.hpp"
#include "../Scenes/Scene.hpp"
#include "../Transformation/DynamicTransformation.hpp"

#include <glm/ext/matrix_float4x4.hpp>

Object::Object(std::shared_ptr<ObjectData> data, ShaderProgram *shaderProgram,
               std::shared_ptr<Transformation> transformations,
               RenderFunction renderFunction)
    : data(data), shaderProgram(shaderProgram),
      transformations(transformations), renderFunction(renderFunction) {}

void Object::assignScene(Scene *scene) {
  this->myScene = scene;
  this->shaderProgram->registerToCamera(scene);
}

void Object::draw(float time) const {
  this->data->setArray();
  this->shaderProgram->setProgram();

  glm::mat4 matrix;

  auto tranPtr = this->transformations.get();

  if (dynamic_cast<DynamicTransformation *>(tranPtr) != nullptr) {
    matrix = static_cast<DynamicTransformation *>(tranPtr)->getMatrix(time);
  } else {
    matrix = tranPtr->getMatrix();
  }

  this->renderFunction(matrix, this->shaderProgram);
}
