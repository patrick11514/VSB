#include "ShaderStorage.hpp"

#include <stdexcept>

ShaderStorage::~ShaderStorage() {
  for (auto &item : this->shaders) {
    delete item.second;
  }
}

void ShaderStorage::addShaderProgram(const std::string &name,
                                     ShaderProgram *shader) {
  printf("EMLACING\n");
  this->shaders.emplace(name, shader);
}
void ShaderStorage::removeShader(const std::string &name) {
  auto it = this->shaders.find(name);

  if (it == this->shaders.end()) {
    return;
  }

  this->shaders.erase(it);
}

const ShaderProgram *
ShaderStorage::getShaderProgram(const std::string &name) const {
  auto it = this->shaders.find(name);

  if (it == this->shaders.end()) {
    throw std::runtime_error("Shader cannot be found");
  }

  return it->second;
}
