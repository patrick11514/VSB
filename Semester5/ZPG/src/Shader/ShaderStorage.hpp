#pragma once

/*
 * File: ShaderStorage.hpp
 * Author: Patrik Mintěl (MIN0150)
 * Description: File contains ShaderStorage class
 */

#include <string>
#include <unordered_map>

#include "ShaderProgram.hpp"

/**
 * Storage to store ShaderPrograms by name
 */
class ShaderStorage {
private:
  std::unordered_map<std::string, std::shared_ptr<ShaderProgram>>
      shaders; ///< Key-Value storage for ShaderPrograms

public:
  /**
   * @brief Add new Shader Program to storage
   * @param name Name of Shader Program
   * @param shader ShaderProgram to be stored
   */
  void addShaderProgram(const std::string &name,
                        std::shared_ptr<ShaderProgram> shader);
  /**
   * @brief Remove ShaderProgram by name
   * @param name Name of Shader Program
   */
  void removeShader(const std::string &name);

  /**
   * @brief Retrieve ShaderProgram by name
   * @param name Name of Shader Program
   * @return ShaderProgram stored under name
   * @throws std::runtime_error when shaderProgram was not found
   */
  std::shared_ptr<ShaderProgram>
  getShaderProgram(const std::string &name) const;
};
