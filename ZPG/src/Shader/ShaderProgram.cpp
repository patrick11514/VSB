
#include "ShaderProgram.hpp"
#include "../Controller.hpp"
#include "../Scenes/Scene.hpp"
#include "Shader.hpp"

#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <stdio.h>

ShaderProgram::ShaderProgram(const Shader &vertexShader,
                             const Shader &fragmentShader,
                             Controller *controller)
    : Observer(), controller(controller) {
  this->programId = glCreateProgram();
  glAttachShader(this->programId, fragmentShader.shaderId);
  glAttachShader(this->programId, vertexShader.shaderId);
  glLinkProgram(this->programId);

  GLint status;
  glGetProgramiv(this->programId, GL_LINK_STATUS, &status);
  if (status == GL_FALSE) {
    printf("ERROR LINK\n");
    GLint infoLogLength;
    glGetProgramiv(this->programId, GL_INFO_LOG_LENGTH, &infoLogLength);
    GLchar *strInfoLog = new GLchar[infoLogLength + 1];
    glGetProgramInfoLog(this->programId, infoLogLength, NULL, strInfoLog);
    fprintf(stderr, "Linker failure: %s\n", strInfoLog);
    delete[] strInfoLog;

    throw std::runtime_error("Linker failure");
  }
}

ShaderProgram::ShaderProgram(const char *vertexShaderPath,
                             const char *fragmentShaderPath,
                             Controller *controller)
    : ShaderProgram(Shader(vertexShaderPath, ShaderType::Vertex),
                    Shader(fragmentShaderPath, ShaderType::Fragment),
                    controller) {}

bool ShaderProgram::checkParameter(const std::string &name) const {
  GLint position = glGetUniformLocation(this->programId, name.c_str());
  return position != -1;
}

void ShaderProgram::setProgram() const { glUseProgram(this->programId); }

void ShaderProgram::resetProgram() { glUseProgram(0); }

void ShaderProgram::registerToCamera(Scene *scene) {
  // Does my shader have view matrix
  if (this->checkParameter("viewMatrix")) {
    // add me as observe
    printf("Registering\n");
    scene->getCamera()->registerObserver(this);
    this->update();
  }
}

bool ShaderProgram::operator==(const ShaderProgram &other) const {
  return this->programId == other.programId;
}

void ShaderProgram::update() {
  this->setProgram();

  auto camera = this->controller->getCamera();

  this->putViewMatrix(camera->getViewMatrix());

  this->putProjectionMatrix(this->controller->getProjectionMatrix());

  if (this->checkParameter("cameraPosition")) {
    this->putCameraPosition(camera->getPosition());
  }

  ShaderProgram::resetProgram();
}

void ShaderProgram::putModelMatrix(const glm::mat4 &matrix) const {
  this->putParameter("modelMatrix", glm::value_ptr(matrix));
}

void ShaderProgram::putViewMatrix(const glm::mat4 &matrix) const {
  this->putParameter("viewMatrix", glm::value_ptr(matrix));
}

void ShaderProgram::putProjectionMatrix(const glm::mat4 &matrix) const {
  this->putParameter("projectionMatrix", glm::value_ptr(matrix));
}

void ShaderProgram::putCameraPosition(const glm::vec3 &vector) const {
  this->putParameter("cameraPosition", glm::value_ptr(vector));
}
