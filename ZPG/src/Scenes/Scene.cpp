#include "Scene.hpp"
#include "../Modifiers/Drawable.hpp"

#include <GLFW/glfw3.h>
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
}

void Scene::registerProgram(ShaderProgram *program) {
  this->usedPrograms.emplace(program);
}

void Scene::render() {
  for (auto *object : this->objects) {
    if (Drawable *drawable = dynamic_cast<Drawable *>(object)) {
      drawable->draw();
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

std::vector<Light *> Scene::getLights() const {
  // TODO: Add support for more lights
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
