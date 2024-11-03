#include "LightBalls.hpp"
#include "../Light/Light.hpp"
#include "../Object/Objects.hpp"
#include "../Transformation/Scale.hpp"
#include "../Transformation/Translate.hpp"

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>
#include <memory>

void LightBalls::addObjects() {
  Camera *camera = new Camera();
  camera->enable();
  this->addObject(camera);

  this->addObject(
      new Light(glm::vec3{1.0f, 1.0f, 1.0f}, glm::vec3{0.f, 0.f, 0.f}));

  this->addObject(createBall(
      this->shaderStorage.getShaderProgram("phong"),
      std::make_shared<Transformation>()
          ->addTransformation(new Translate(glm::vec3(0.f, 0.f, 2.f)))
          ->addTransformation(new Scale({.1f, .1f, .1f}))));

  this->addObject(createBall(
      this->shaderStorage.getShaderProgram("phong"),
      std::make_shared<Transformation>()
          ->addTransformation(new Translate(glm::vec3(0.f, 0.f, -2.f)))
          ->addTransformation(new Scale({.1f, .1f, .1f}))));

  this->addObject(createBall(
      this->shaderStorage.getShaderProgram("phong"),
      std::make_shared<Transformation>()
          ->addTransformation(new Translate(glm::vec3(0.f, 2.f, 0.f)))
          ->addTransformation(new Scale({.1f, .1f, .1f}))));

  this->addObject(createBall(
      this->shaderStorage.getShaderProgram("phong"),
      std::make_shared<Transformation>()
          ->addTransformation(new Translate(glm::vec3(0.f, -2.f, 0.f)))
          ->addTransformation(new Scale({.1f, .1f, .1f}))));
}
