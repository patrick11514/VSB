
#include "ShaderProgram.hpp"
#include "../Controller.hpp"
#include "../Light/Light.hpp"
#include "../Scenes/Scene.hpp"
#include "Shader.hpp"

#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
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
//: ShaderProgram(Shader(vertexShaderPath, ShaderType::Vertex),
//              Shader(fragmentShaderPath, ShaderType::Fragment),
//              controller) {}
{
  this->controller = controller;
  this->programId = this->loadShader(vertexShaderPath, fragmentShaderPath);
}

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
    // TODO: extend support for more cameras
    auto *camera = scene->getCamera();

    if (camera == nullptr) {
      printf("Scene without enabled camera\n");
      return;
    }

    camera->registerObserver(this);
    this->update(camera);
  }
}

void ShaderProgram::registerToLight(Scene *scene) {
  // Does my shader have light
  if (this->checkParameter("lightPosition") &&
      this->checkParameter("lightColor")) {
    // add me as observe
    // TODO: extend support for more lights
    auto *light = scene->getLight();

    if (light == nullptr) {
      printf("Scene without light\n");
      return;
    }

    light->registerObserver(this);
    this->update(light);
  }
}

bool ShaderProgram::operator==(const ShaderProgram &other) const {
  return this->programId == other.programId;
}

void ShaderProgram::update(const Observable *who) {
  this->setProgram();

  if (dynamic_cast<const Camera *>(who) != nullptr) {
    auto *camera = static_cast<const Camera *>(who);

    this->putViewMatrix(camera->getViewMatrix());

    this->putProjectionMatrix(this->controller->getProjectionMatrix());

    if (this->checkParameter("cameraPosition")) {
      this->putCameraPosition(camera->getPosition());
    }
  } else if (dynamic_cast<const Light *>(who) != nullptr) {
    // update lightning
    auto *light = static_cast<const Light *>(who);

    this->putLightPosition(light->getPosition());
    this->putLightColor(light->getColor());
  }

  ShaderProgram::resetProgram();
}

void ShaderProgram::putModelMatrix(const glm::mat4 &matrix) const {
  this->putParameter("modelMatrix", matrix);
}

void ShaderProgram::putViewMatrix(const glm::mat4 &matrix) const {
  this->putParameter("viewMatrix", matrix);
}

void ShaderProgram::putProjectionMatrix(const glm::mat4 &matrix) const {
  this->putParameter("projectionMatrix", matrix);
}

void ShaderProgram::putCameraPosition(const glm::vec3 &vector) const {
  this->putParameter("cameraPosition", vector);
}

void ShaderProgram::putLightPosition(const glm::vec3 &vector) const {
  this->putParameter("lightPosition", vector);
}

void ShaderProgram::putLightColor(const glm::vec3 &vector) const {
  this->putParameter("lightColor", vector);
}
