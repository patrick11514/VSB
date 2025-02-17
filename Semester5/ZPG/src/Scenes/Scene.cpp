#include "Scene.hpp"

#include <GLFW/glfw3.h>
#include <algorithm>
#include <glm/ext/matrix_float4x4.hpp>

Scene::~Scene() {
  for (auto *obj : this->objects) {
    delete obj;
  }
}

void Scene::addObject(BaseObject *object) {
  object->assignScene(this);
  this->objects.push_back(object);

  // assing light id
  if (Light *light = dynamic_cast<Light *>(object)) {
    light->assignId(this->lightCount++);
  }

  if (SkyBox *skybox = dynamic_cast<SkyBox *>(object)) {
    auto cameras = this->getCameras();
    for (auto &camera : cameras) {
      camera->registerObserver(skybox);
    }
  }

  if (Camera *camera = dynamic_cast<Camera *>(object)) {
    auto *skybox = this->getSkybox();
    if (skybox != nullptr) {
      camera->registerObserver(skybox);
    }
  }
}

void Scene::removeObject(BaseObject *object) {
  this->objects.erase(
      std::find(this->objects.begin(), this->objects.end(), object));
}

void Scene::registerProgram(ShaderProgram *program) {
  this->usedPrograms.emplace(program);
}

void Scene::render() {
  for (auto *object : this->objects) {
    if (Drawable *drawable = dynamic_cast<Drawable *>(object)) {
      if (dynamic_cast<SkyBox *>(object) == nullptr) {
        drawable->draw();
      }
    }
  }
}

Camera *Scene::getCamera() {
  for (auto *object : this->objects) {
    if (dynamic_cast<Camera *>(object) == nullptr)
      continue;

    if (!static_cast<Camera *>(object)->getStatus())
      continue;

    return static_cast<Camera *>(object);
  }

  // no camera found
  return nullptr;
}

std::vector<Camera *> Scene::getCameras() {
  std::vector<Camera *> cameras;
  for (auto *object : this->objects) {
    if (dynamic_cast<Camera *>(object) == nullptr)
      continue;

    cameras.emplace_back(static_cast<Camera *>(object));
  }

  // no light found
  return cameras;
}

std::vector<Light *> Scene::getLights() const {
  std::vector<Light *> lights;
  for (auto *object : this->objects) {
    if (dynamic_cast<Light *>(object) == nullptr)
      continue;

    lights.emplace_back(static_cast<Light *>(object));
  }

  // no light found
  return lights;
}

Light *Scene::getLight(int id) const {
  //@TODO
  for (auto *object : this->objects) {
    if (auto *light = dynamic_cast<Light *>(object)) {
      if (light->getId() == id)
        return light;
    }
  }

  return nullptr;
}

SkyBox *Scene::getSkybox() const {
  for (auto *object : this->objects) {
    if (dynamic_cast<SkyBox *>(object) == nullptr)
      continue;

    return static_cast<SkyBox *>(object);
  }

  // no camera found
  return nullptr;
}

void Scene::activate() {
  this->getCamera()->notifyObservers();

  auto lights = this->getLights();

  for (auto *light : lights) {
    light->notifyObservers();
  }

  for (auto *shader : this->usedPrograms) {
    shader->putLightCount(static_cast<int>(lights.size()));
    for (auto *light : lights) {
      shader->putLightProperties(light);
    }
  }
}

Drawable *Scene::getObject(int id) {
  for (auto *obj : this->objects) {
    if (auto *drawable = dynamic_cast<Drawable *>(obj)) {
      if (drawable->getId() == id) {
        return drawable;
      }
    }
  }

  return nullptr;
}
