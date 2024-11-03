#include "Light.hpp"
#include "../Object/Objects.hpp"
#include "../Transformation/Scale.hpp"
#include "../Transformation/Translate.hpp"

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>
#include <memory>

void Light::addObjects() {

  auto scale = new Scale({.1f, .1f, .1f});

  this->addObject(createBall(
      this->shaderStorage.getShaderProgram("green"),
      std::make_shared<Transformation>()
          ->addTransformation(new Translate(glm::vec3(0.f, 0.f, 2.f)))
          ->addTransformation(scale)));

  this->addObject(createBall(
      this->shaderStorage.getShaderProgram("lambert"),
      std::make_shared<Transformation>()
          ->addTransformation(new Translate(glm::vec3(0.f, 0.f, -2.f)))
          ->addTransformation(scale)));

  this->addObject(createBall(
      this->shaderStorage.getShaderProgram("phong"),
      std::make_shared<Transformation>()
          ->addTransformation(new Translate(glm::vec3(0.f, 2.f, 0.f)))
          ->addTransformation(scale)));

  this->addObject(createBall(
      this->shaderStorage.getShaderProgram("blinnphong"),
      std::make_shared<Transformation>()
          ->addTransformation(new Translate(glm::vec3(0.f, -2.f, 0.f)))
          ->addTransformation(scale)));
}
