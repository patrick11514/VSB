#include "DarkForest.hpp"
#include "../Light/BallLight.hpp"
#include "../Light/Flashlight.hpp"
#include "../Object/Material/DarkBushMaterial.hpp"
#include "../Object/Material/DarkGrassMaterial.hpp"
#include "../Object/Material/DarkTreeMaterial.hpp"
#include "../Object/Material/SolidWhiteMaterial.hpp"
#include "../Object/Objects.hpp"
#include "../Transformation/DynamicRotation.hpp"
#include "../Transformation/RandomTranslate.hpp"
#include "../Transformation/Scale.hpp"
#include "../Transformation/Translate.hpp"

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include <random>

void DarkForest::addObjects() {
  auto *phong = this->shaderStorage.getShaderProgram("phong").get();
  auto *blinnPhong = this->shaderStorage.getShaderProgram("blinnphong").get();

  Camera *camera = new Camera();
  camera->enable();
  this->addObject(camera);

  this->addObject(new Flashlight(glm::vec3{0.5, 0.5, 0.5}, this->getCamera(),
                                 1.0, 0.03, 0.00005));

  std::random_device dev;
  std::mt19937 rng(dev());

  std::uniform_real_distribution<float> x(-50, 50);
  std::uniform_real_distribution<float> z(-50, 50);
  std::uniform_real_distribution<float> scale(0.2, 0.4);
  std::uniform_real_distribution<float> rotation(0, 360);
  std::uniform_real_distribution<float> rotationFactor(-0.01, 0.01);

  for (int i = 0; i < 5; ++i) {
    float xCoord = x(rng) * 5;
    float zCoord = z(rng) * 5;
    auto tran = std::make_shared<Transformation>()
                    ->addTransformation(
                        new RandomTranslate(glm::vec3(xCoord, 20, zCoord)))
                    ->addTransformation(new Scale(glm::vec3{0.1}));
    this->addObject(new BallLight(glm::vec3(0.5, 0.5, 0.5), tran, 1.0, 0.5,
                                  0.05, phong,
                                  std::make_shared<SolidWhiteMaterial>()));
  }

  // clang-format off
  Model model(std::vector<float>{
    -1.0f, 0.0f, 1.0f,    0.f, 1.f, 0.f, 0.f  , 200.f,     // Top Left
     1.0f, 0.0f, 1.0f,    0.f, 1.f, 0.f, 200.f, 200.f,    // Top Right
     1.0f, 0.0f, -1.0f,   0.f, 1.f, 0.f, 200.f, 0.f,    // Bottom Right
    -1.0f, 0.0f, -1.0f,   0.f, 1.f, 0.f, 0.f  , 0.f,    // Bottom Left
  });
  // clang-format on

  auto tran = std::make_shared<Transformation>();
  tran->addTransformation(new Scale(glm::vec3(200.f, 1.f, 200.f)));

  DrawableObject *obj = new DrawableObject(
      std::make_shared<ObjectData>(
          model, 3,
          []() {
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                                  (GLvoid *)0);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                                  (GLvoid *)(3 * sizeof(float)));
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                                  (GLvoid *)(6 * sizeof(float)));
          }),
      phong, tran, []() { glDrawArrays(GL_QUADS, 0, 4); },
      this->textureStorage.getTexture("dark_grass"));

  this->addObject(obj);

  auto treeMaterial = std::make_shared<DarkTreeMaterial>();
  auto bushMaterial = std::make_shared<DarkBushMaterial>();

  for (int i = 0; i < 100; ++i) {
    float s = scale(rng);
    float xCoord = x(rng);
    float zCoord = z(rng);
    float rotate = rotation(rng);

    auto tran = std::make_shared<Transformation>();
    tran->addTransformation(new DynamicRotation(rotate,
                                                glm::vec3(0.f, 1.f, 0.f),
                                                rotationFactor(rng)))
        ->addTransformation(new Scale(glm::vec3(s)))
        ->addTransformation(new Translate(glm::vec3(xCoord, 0, zCoord)));

    auto tree = createTree(blinnPhong, tran, treeMaterial);

    /*tree.setAnimationFunction([](const glm::mat4x4 &tran, float time) {
      auto rotation = Rotation(time, glm::vec3(1.f, 1.f, 1.f));
      return tran * rotation.getMatrix();
    });*/

    this->addObject(tree);
  }

  for (int i = 0; i < 100; ++i) {
    float s = 0.5 + scale(rng);
    float xCoord = x(rng);
    float zCoord = z(rng);

    auto tran = std::make_shared<Transformation>();
    tran->addTransformation(new Scale(glm::vec3(s)));
    tran->addTransformation(new Translate(glm::vec3(xCoord, 0, zCoord)));

    this->addObject(createBush(blinnPhong, tran, bushMaterial));
  }
}
