#include "Forest.hpp"
#include "../Factories/ObjectFactory.hpp"
#include "../Light/BallLight.hpp"
#include "../Light/DirectionalLight.hpp"
#include "../Light/Flashlight.hpp"
#include "../Light/ReflectorLight.hpp"
#include "../Object/Material/BushMaterial.hpp"
#include "../Object/Material/GrassMaterial.hpp"
#include "../Object/Material/SolidWhiteMaterial.hpp"
#include "../Object/Material/TreeMaterial.hpp"
#include "../Object/Objects.hpp"
#include "../Object/SkyBox.hpp"
#include "../Transformation/BezierMovement.hpp"
#include "../Transformation/DynamicRotation.hpp"
#include "../Transformation/LineMovement.hpp"
#include "../Transformation/RandomTranslate.hpp"
#include "../Transformation/Scale.hpp"
#include "../Transformation/Translate.hpp"

#include <glm/ext/matrix_float4x3.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include <random>

void Forest::addObjects() {
  auto *phong = this->shaderStorage.getShaderProgram("phong").get();
  auto *blinnPhong = this->shaderStorage.getShaderProgram("blinnphong").get();

  Camera *camera = new Camera();
  camera->enable();
  this->addObject(camera);
  this->addObject(new BallLight(
      glm::vec3(1.0, 1.0, 1.0),
      std::make_shared<Transformation>()
          ->addTransformation(new Translate(glm::vec3{4.0, 2.0, 4.0}))
          ->addTransformation(new Scale(glm::vec3{0.2})),
      1.0, 0.09, 0.032, phong, std::make_shared<SolidWhiteMaterial>()));
  this->addObject(new DirectionalLight(
      glm::vec3(1.0, 0.0, 0.0), glm::vec3{1.0, 0.0, 0.0}, 0.0, 0.0, 0.0));

  this->addObject(new ReflectorLight(
      glm::vec3(0.0, 1.0, 0.0), glm::vec3{0.0, -1.0, 0.0}, 30.f,
      std::make_shared<Transformation>()->addTransformation(
          new Translate(glm::vec3{20.0, 5.0, 0.0})),
      0.0, 0.0, 0.0));

  this->addObject(new Flashlight(glm::vec3{1.0, 0.0, 0.0}, this->getCamera(),
                                 1.0, 0.03, 0.00005));

  this->addObject(new SkyBox(
      std::array<std::string, 6>{"../textures/posx.jpg", "../textures/negx.jpg",
                                 "../textures/posy.jpg", "../textures/negy.jpg",
                                 "../textures/posz.jpg",
                                 "../textures/negz.jpg"},
      this->shaderStorage.getShaderProgram("skybox").get()));

  this->addObject(
      ObjectFactory::objObject()
          ->loadModel("../objects/house.obj")
          ->defaultSlice()
          ->finish(this->shaderStorage.getShaderProgram("phong").get(),
                   std::make_shared<Transformation>()->addTransformation(
                       new Translate(glm::vec3{10.f, 0.f, 10.f})),
                   this->textureStorage.getTexture("house")));

  this->addObject(
      ObjectFactory::objObject()
          ->loadModel("../objects/login.obj")
          ->defaultSlice()
          ->finish(this->shaderStorage.getShaderProgram("phong").get(),
                   std::make_shared<Transformation>()->addTransformation(
                       new Translate(glm::vec3{-10.f, 0.f, 10.f})),
                   this->textureStorage.getTexture("planks")));

  this->addObject(
      ObjectFactory::objObject()
          ->loadModel("../objects/Kafka.obj")
          ->defaultSlice()
          ->finish(this->shaderStorage.getShaderProgram("phong").get(),
                   std::make_shared<Transformation>()->addTransformation(
                       new Translate(glm::vec3{-10.f, 0.f, -10.f})),
                   this->textureStorage.getTexture("kafka")));

  this->addObject(
      ObjectFactory::objObject()
          ->loadModel("../objects/zombie.obj")
          ->defaultSlice()
          ->finish(this->shaderStorage.getShaderProgram("phong").get(),
                   std::make_shared<Transformation>()->addTransformation(
                       new LineMovement(glm::vec3{-5, 1, -5},
                                        glm::vec3{5, 1, 8}, 5)),
                   this->textureStorage.getTexture("zoombie")));

  this->addObject(
      ObjectFactory::objObject()
          ->loadModel("../objects/zombie.obj")
          ->defaultSlice()
          ->finish(this->shaderStorage.getShaderProgram("phong").get(),
                   std::make_shared<Transformation>()->addTransformation(
                       new BezierMovement<glm::mat4x3>(
                           glm::mat4x3{glm::vec3{-1, 0, 0}, glm::vec3{0, 1, 0},
                                       glm::vec3{0, -1, 0}, glm::vec3{1, 0, 0}},
                           5)),
                   this->textureStorage.getTexture("zoombie")));

  std::random_device dev;
  std::mt19937 rng(dev());

  std::uniform_real_distribution<float> x(-50, 50);
  std::uniform_real_distribution<float> z(-50, 50);
  std::uniform_real_distribution<float> scale(0.2, 0.4);
  std::uniform_real_distribution<float> rotation(0, 360);
  std::uniform_real_distribution<float> rotationFactor(-0.01, 0.01);

  for (int i = 0; i < 10; ++i) {
    float xCoord = x(rng) * 20;
    float zCoord = z(rng) * 20;
    auto tran = std::make_shared<Transformation>()
                    ->addTransformation(
                        new RandomTranslate(glm::vec3(xCoord, 20, zCoord)))
                    ->addTransformation(new Scale(glm::vec3{0.1}));
    this->addObject(new BallLight(glm::vec3(0.5, 0.5, 0.5), tran, 1.0, 0.05,
                                  0.00001, phong,
                                  std::make_shared<SolidWhiteMaterial>()));
  }

  this->addObject(
      createPlane(phong,
                  std::make_shared<Transformation>()->addTransformation(
                      new Scale(glm::vec3(200.f, 1.f, 200.f))),
                  this->textureStorage.getTexture("grass")));

  this->addObject(
      createPlaneNormal(phong,
                        std::make_shared<Transformation>()->addTransformation(
                            new Translate(glm::vec3(5.f, 5.f, 5.f))),
                        this->textureStorage.getTexture("planks")));

  auto treeMaterial = std::make_shared<TreeMaterial>();
  auto bushMaterial = std::make_shared<BushMaterial>();

  auto tree = createTree(
      blinnPhong,
      std::make_shared<Transformation>()
          ->addTransformation(
              new DynamicRotation(10.f, glm::vec3(0.f, 1.f, 0.f), 0.01))
          ->addTransformation(new Translate(glm::vec3(2.0, 2.0, 2.0))),
      treeMaterial);

  this->addObject(tree);

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

    tree->assignId();

    tree.setAnimationFunction([](const glm::mat4x4 &tran, float time) {
      auto rotation = Rotation(time, glm::vec3(1.f, 1.f, 1.f));
      return tran * rotation.getMatrix();
    });

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
