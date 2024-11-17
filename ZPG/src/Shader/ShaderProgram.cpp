
#include "ShaderProgram.hpp"
#include "../Controller.hpp"
#include "../Light/DirectionalLight.hpp"
#include "../Light/ReflectorLight.hpp"
#include "../Scenes/Scene.hpp"
#include "Shader.hpp"

#include <GLFW/glfw3.h>
#include <format>
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
  if (this->checkParameter("lightCount")) {
    // add me as observe
    // TODO: extend support for more lights
    auto lights = scene->getLights();

    if (lights.size() == 0) {
      printf("Scene without light\n");
      return;
    }

    this->putParameter("lightCount", static_cast<int>(lights.size()));

    for (auto *light : lights) {
      light->registerObserver(this);

      this->putLightProperties(light); // only once
      this->update(light);
    }
  }
}

bool ShaderProgram::operator==(const ShaderProgram &other) const {
  return this->programId == other.programId;
}

void ShaderProgram::update(const Observable *who) {
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

    this->putLightPosition(light);
    // this->putLightProperties(light);
  }
}

void ShaderProgram::putModelMatrix(
    const Transformation *transformations) const {
  this->putParameter("modelMatrix", transformations->getMatrix());
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

void ShaderProgram::putLightPosition(const Light *light) const {
  this->putParameter(std::format("lights[{}].lightMatrix", light->getId()),
                     light->getTransformations()->getMatrix());

  if (const auto *directionalLight =
          dynamic_cast<const DirectionalLight *>(light)) {
    this->putParameter(std::format("lights[{}].direction", light->getId()),
                       directionalLight->getDirection());
  } else if (const auto *reflectorLight =
                 dynamic_cast<const ReflectorLight *>(light)) {
    this->putParameter(std::format("lights[{}].direction", light->getId()),
                       reflectorLight->getDirection());

    this->putParameter(std::format("lights[{}].angle", light->getId()),
                       reflectorLight->getAngle());
  } else {
  }
}

void ShaderProgram::putLightProperties(const Light *light) const {
  this->putParameter(std::format("lights[{}].type", light->getId()),
                     static_cast<int>(light->getType()));
  this->putParameter(std::format("lights[{}].color", light->getId()),
                     light->getColor());
  this->putParameter(std::format("lights[{}].kc", light->getId()),
                     light->getKc());
  this->putParameter(std::format("lights[{}].kl", light->getId()),
                     light->getKl());
  this->putParameter(std::format("lights[{}].kq", light->getId()),
                     light->getKq());
}

void ShaderProgram::putMaterial(const Material *material) const {
  this->putParameter("material.ra", material->getRa());
  this->putParameter("material.rs", material->getRs());
  this->putParameter("material.rd", material->getRd());
}
