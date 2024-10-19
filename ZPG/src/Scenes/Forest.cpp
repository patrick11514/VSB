#include "Forest.hpp"
#include "../Object/Objects.hpp"
#include "../Transformation/Rotation.hpp"
#include "../Transformation/Scale.hpp"
#include "../Transformation/Translate.hpp"
#include "Scene.hpp"

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>
#include <random>

Forest::Forest(const ShaderStorage &shaderStorage) : Scene() {
  std::random_device dev;
  std::mt19937 rng(dev());

  std::uniform_real_distribution<float> x(-20, 20);
  std::uniform_real_distribution<float> y(0, 1);
  std::uniform_real_distribution<float> z(-20, 20);
  std::uniform_real_distribution<float> scale(0.01, 0.2);

  for (int i = 0; i < 100; ++i) {
    float s = scale(rng);
    float xCoord = x(rng);
    float yCoord = y(rng);
    float zCoord = z(rng);

    printf("%f %f %f (%f)\n", xCoord, yCoord, zCoord, s);

    Transformations tran;
    tran.addTransformation(Scale(glm::vec3(0.1)));
    tran.addTransformation(
        Translate(glm::vec3(xCoord * 4, yCoord * 4, zCoord * 4)));

    auto tree =
        createTree(shaderStorage.getShaderProgram("greenByCoord"), tran);

    tree.setAnimationFunction([](const glm::mat4x4 &tran, float time) {
      auto rotation = Rotation(time, glm::vec3(1.f, 1.f, 1.f));
      return tran * rotation.getMatrix();
    });

    this->addObject(tree);
  }

  for (int i = 0; i < 80; ++i) {
    float s = 0.5 + scale(rng);
    float xCoord = x(rng);
    float yCoord = y(rng);
    float zCoord = z(rng);

    printf("%f %f %f (%f)\n", xCoord, yCoord, zCoord, s);

    Transformations tran;
    tran.addTransformation(Scale(glm::vec3(s)));
    tran.addTransformation(Translate(glm::vec3(xCoord, yCoord, zCoord)));

    this->addObject(createBush(shaderStorage.getShaderProgram("idk"), tran));
  }
}
