#include "SkyBox.hpp"
#include "BaseObject.hpp"
#include "Models/skycube.h"
#include "Texture/BaseTexture.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <glm/matrix.hpp>

SkyBox::SkyBox(std::array<std::string, 6> faces, ShaderProgram *shaderProgram)
    : BaseTexture(faces, GL_TEXTURE_CUBE_MAP), shaderProgram(shaderProgram) {
  this->data = std::make_shared<ObjectData>(
      Model{skycube, sizeof(skycube) / sizeof(float)});
}

void SkyBox::update(Observable *who) {
  if (const auto *camera = dynamic_cast<Camera *>(who)) {
    this->modelMatrix = glm::translate(glm::mat4{1.f}, camera->getPosition());
  }
}

void SkyBox::assignScene(Scene *scene) {
  BaseObject::assignScene(scene);
  this->shaderProgram->registerToCamera(scene);
}

void SkyBox::draw() const {
  this->data->bindArray();

  this->shaderProgram->putTextureUnit(this->getUnit());
  this->shaderProgram->putRawModelMatrix(this->modelMatrix);

  this->shaderProgram->setProgram();
  glDrawArrays(GL_TRIANGLES, 0, 108);
  ShaderProgram::resetProgram();
}
