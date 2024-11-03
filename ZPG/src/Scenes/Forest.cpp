#include "Forest.hpp"
#include "../Object/Objects.hpp"
#include "../Transformation/Rotation.hpp"
#include "../Transformation/Scale.hpp"
#include "../Transformation/Translate.hpp"

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>
#include <random>

void Forest::addObjects() {
  std::random_device dev;
  std::mt19937 rng(dev());

  std::uniform_real_distribution<float> x(-50, 50);
  std::uniform_real_distribution<float> z(-50, 50);
  std::uniform_real_distribution<float> scale(0.2, 0.4);
  std::uniform_real_distribution<float> rotation(0, 360);

  for (int i = 0; i < 500; ++i) {
    float s = scale(rng);
    float xCoord = x(rng);
    float zCoord = z(rng);
    float rotate = rotation(rng);

    auto tran = std::make_shared<Transformation>();
    tran->addTransformation(new Scale(glm::vec3(s)))
        ->addTransformation(new Translate(glm::vec3(xCoord, 0, zCoord)))
        ->addTransformation(new Rotation(rotate, glm::vec3(0.f, 1.f, 0.f)));

    auto tree =
        createTree(this->shaderStorage.getShaderProgram("greenByCoord"), tran);

    /*tree.setAnimationFunction([](const glm::mat4x4 &tran, float time) {
      auto rotation = Rotation(time, glm::vec3(1.f, 1.f, 1.f));
      return tran * rotation.getMatrix();
    });*/

    this->addObject(tree);
  }

  for (int i = 0; i < 500; ++i) {
    float s = 0.5 + scale(rng);
    float xCoord = x(rng);
    float zCoord = z(rng);

    auto tran = std::make_shared<Transformation>();
    tran->addTransformation(new Scale(glm::vec3(s)));
    tran->addTransformation(new Translate(glm::vec3(xCoord, 0, zCoord)));

    this->addObject(
        createBush(this->shaderStorage.getShaderProgram("idk"), tran));
  }
}
