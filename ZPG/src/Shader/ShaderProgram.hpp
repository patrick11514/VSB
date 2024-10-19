#pragma once

#include "../Patterns/Observer.hpp"
#include "Shader.hpp"

#include <stdexcept>

class Controller;

class ShaderProgram : public Observer {
private:
  GLuint programId;
  glm::mat4 viewMatrix;
  glm::mat4 projectionMatrix;
  Controller *controller;

public:
  ShaderProgram(Shader &vertexShader, Shader &fragmentShader,
                Controller *controller);

  ~ShaderProgram();

  void setProgram() const;

  static void resetProgram();

  bool checkParameter(const std::string &name) const;

  template <typename T>
  void putParameter(const std::string &name, T &value) const {
    GLint position = glGetUniformLocation(this->programId, name.c_str());
    if (position == -1) {
      throw std::runtime_error("Unable to find modelMatrix position");
    }

    glUniformMatrix4fv(position, 1, GL_FALSE, &value);
  }

  // operators
  bool operator==(const ShaderProgram &other) const;

  void update(glm::mat4 &projectionMatrix) override;

  void call() const override;
};
