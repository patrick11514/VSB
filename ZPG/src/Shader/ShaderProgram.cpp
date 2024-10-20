#include "ShaderProgram.hpp"
#include "../Controller.hpp"
#include "Shader.hpp"

#include <GLFW/glfw3.h>
#include <stdio.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

ShaderProgram::ShaderProgram(const Shader &vertexShader,
                             const Shader &fragmentShader,
                             Controller *controller)
    : Observer(), controller(controller) {
  printf("CREATE\n");
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

  // Does my shader have view matrix
  if (this->checkParameter("viewMatrix")) {
    // add me as observer
    this->controller->getCamera().addObserver(this);

    this->viewMatrix = this->controller->getCamera().calculateViewMatrix();
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

void ShaderProgram::setProgram() const {
  glUseProgram(this->programId);
  if (this->checkParameter("viewMatrix")) {
    // std::cout << "Puttin viewMatrix " << glm::to_string(this->viewMatrix)
    //           << std::endl;
    this->putParameter("viewMatrix", this->viewMatrix[0][0]);
  }
  if (this->checkParameter("projectionMatrix")) {
    // std::cout << "Puttin projectionMatrix "
    //           << glm::to_string(this->projectionMatrix) << std::endl;
    this->putParameter("projectionMatrix",
                       this->controller->getProjectionMatrix()[0][0]);
  }
}

void ShaderProgram::resetProgram() { glUseProgram(0); }

bool ShaderProgram::operator==(const ShaderProgram &other) const {
  return this->programId == other.programId;
}

void ShaderProgram::update(glm::mat4 &viewMatrix) {
  this->viewMatrix = viewMatrix;
}
