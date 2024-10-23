#include "Light.hpp"
#include "../Object/Objects.hpp"
#include "../Transformation/Scale.hpp"
#include "../Transformation/Translate.hpp"
#include "Scene.hpp"

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>
#include <memory>

Light::Light(const ShaderStorage &shaderStorage) : Scene() {

  auto scale = new Scale({.1f, .1f, .1f});

  this->addObject(createBall(
      shaderStorage.getShaderProgram("light"),
      std::make_shared<Transformation>()
          ->addTransformation(new Translate(glm::vec3(0.f, 0.f, 2.f)))
          ->addTransformation(scale)));

  this->addObject(createBall(
      shaderStorage.getShaderProgram("light"),
      std::make_shared<Transformation>()
          ->addTransformation(new Translate(glm::vec3(0.f, 0.f, -2.f)))
          ->addTransformation(scale)));

  this->addObject(createBall(
      shaderStorage.getShaderProgram("light"),
      std::make_shared<Transformation>()
          ->addTransformation(new Translate(glm::vec3(0.f, 2.f, 0.f)))
          ->addTransformation(scale)));

  this->addObject(createBall(
      shaderStorage.getShaderProgram("light"),
      std::make_shared<Transformation>()
          ->addTransformation(new Translate(glm::vec3(0.f, -2.f, 0.f)))
          ->addTransformation(scale)));
}
