#include "Forest.hpp"
#include "../Object/Objects.hpp"
#include "../Transformation/DynamicRotation.hpp"
#include "../Transformation/DynamicTransformation.hpp"
#include "../Transformation/Rotation.hpp"
#include "../Transformation/Scale.hpp"
#include "../Transformation/Translate.hpp"

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <random>

void Forest::addObjects() {
  // clang-format off
    Model model(std::vector<float>{

    -1.0f, 0.0f, 1.0f,    // Top Left
     1.0f, 0.0f, 1.0f,    // Top Right
     1.0f, 0.0f, -1.0f,   // Bottom Right
    -1.0f, 0.0f, -1.0f    // Bottom Left
  });
  // clang-format on
  auto tran = std::make_shared<Transformation>();
  tran->addTransformation(new Scale(glm::vec3(200.f, 1.f, 200.f)));

  DrawableObject obj(
      std::make_shared<ObjectData>(model),
      this->shaderStorage.getShaderProgram("green"), tran,
      [&](const glm::mat4x4 &matrix, const ShaderProgram *shaderProgram) {
        shaderProgram->putParameter("modelMatrix", glm::value_ptr(matrix));
        glDrawArrays(GL_QUADS, 0, 4);
      });

  this->addObject(obj);

  std::random_device dev;
  std::mt19937 rng(dev());

  std::uniform_real_distribution<float> x(-50, 50);
  std::uniform_real_distribution<float> z(-50, 50);
  std::uniform_real_distribution<float> scale(0.2, 0.4);
  std::uniform_real_distribution<float> rotation(0, 360);
  std::uniform_real_distribution<float> rotationFactor(-0.01, 0.01);

  for (int i = 0; i < 500; ++i) {
    float s = scale(rng);
    float xCoord = x(rng);
    float zCoord = z(rng);
    float rotate = rotation(rng);

    auto tran = std::make_shared<DynamicTransformation>();
    tran->addTransformation(new DynamicRotation(rotate,
                                                glm::vec3(0.f, 1.f, 0.f),
                                                rotationFactor(rng)))
        ->addTransformation(new Scale(glm::vec3(s)))
        ->addTransformation(new Translate(glm::vec3(xCoord, 0, zCoord)));

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
