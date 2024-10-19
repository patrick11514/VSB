#pragma once

#include <string>
#include <unordered_map>

#include "ShaderProgram.hpp"

class ShaderStorage {
private:
  std::unordered_map<std::string, ShaderProgram *> shaders;

public:
  ~ShaderStorage();

  void addShaderProgram(const std::string &name, ShaderProgram *shader);
  void removeShader(const std::string &name);

  const ShaderProgram *getShaderProgram(const std::string &name) const;
};
