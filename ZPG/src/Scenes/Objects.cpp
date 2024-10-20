#include "../Object/Objects.hpp"
#include "../Transformation/Scale.hpp"
#include "../Transformation/Translate.hpp"
#include "Objects.hpp"
#include "Scene.hpp"

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>

Objects::Objects(const ShaderStorage &shaderStorage) : Scene() {

  this->addObject(createBall(
      shaderStorage.getShaderProgram("MatShader"),
      std::make_shared<Transformation>()
          ->addTransformation(new Scale(glm::vec3(0.2f)))
          ->addTransformation(new Translate(glm::vec3(1.f, 0.f, 0.f)))));

  this->addObject(
      createBush(shaderStorage.getShaderProgram("blue"),
                 std::make_shared<Transformation>()->addTransformation(
                     new Translate(glm::vec3(-0.5f, 0.5f, 0.f)))));

  this->addObject(
      createTree(shaderStorage.getShaderProgram("red"),
                 std::make_shared<Transformation>()->addTransformation(
                     new Scale(glm::vec3(.1f)))));
}
