#include "../Object/Objects.hpp"
#include "../Object/Material/SolidBlueMaterial.hpp"
#include "../Object/Material/SolidGreenMaterial.hpp"
#include "../Object/Material/SolidPurpleMaterial.hpp"
#include "../Object/Material/SolidRedMaterial.hpp"
#include "../Transformation/Scale.hpp"
#include "../Transformation/Translate.hpp"
#include "Objects.hpp"

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>
#include <memory>

void Objects::addObjects() {
  auto *phong = this->shaderStorage.getShaderProgram("phong").get();

  Camera *camera = new Camera();
  camera->enable();
  this->addObject(camera);

  this->addObject(createBall(
      phong,
      std::make_shared<Transformation>()
          ->addTransformation(new Scale(glm::vec3(0.2f)))
          ->addTransformation(new Translate(glm::vec3(1.f, 0.f, 0.f))),
      std::make_shared<SolidBlueMaterial>()));

  this->addObject(
      createBush(phong,
                 std::make_shared<Transformation>()->addTransformation(
                     new Translate(glm::vec3(-0.5f, 0.5f, 0.f))),
                 std::make_shared<SolidRedMaterial>()));

  this->addObject(
      createTree(phong,
                 std::make_shared<Transformation>()->addTransformation(
                     new Scale(glm::vec3(.1f))),
                 std::make_shared<SolidPurpleMaterial>()));

  this->addObject(createGift(phong, std::make_shared<Transformation>(),
                             std::make_shared<SolidGreenMaterial>()));
}
