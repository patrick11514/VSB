#pragma once

#include <unordered_map>
#include <string>

#include "ShaderProgram.hpp"

class ShaderStorage
{
private:
    std::unordered_map<std::string, ShaderProgram> shaders;

public:
    void addShaderProgram(const std::string &name, ShaderProgram &shader);
    void removeShader(const std::string &name);

    const ShaderProgram &getShaderProgram(const std::string &name) const;
};