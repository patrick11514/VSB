#include "ShaderStorage.hpp"

#include <stdexcept>

void ShaderStorage::addShaderProgram(const std::string &name,
                                     std::shared_ptr<ShaderProgram> shader) {
  this->shaders.emplace(name, shader);
  printf("Shader %s is %d\n", name.c_str(), shader->programId);
}
void ShaderStorage::removeShader(const std::string &name) {
  auto it = this->shaders.find(name);

  if (it == this->shaders.end()) {
    return;
  }

  this->shaders.erase(it);
}

std::shared_ptr<ShaderProgram>
ShaderStorage::getShaderProgram(const std::string &name) const {
  auto it = this->shaders.find(name);

  if (it == this->shaders.end()) {
    throw std::runtime_error("Shader " + name + " cannot be found");
  }

  return it->second;
}
