#include "Scene.hpp"
#include "../Light/Light.hpp"
#include "../Object/DrawableObject.hpp"

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
}

void Scene::render() const {
  float time = glfwGetTime();

  for (const auto *object : this->objects) {
    if (dynamic_cast<const DrawableObject *>(object) != nullptr) {
      static_cast<const DrawableObject *>(object)->draw(time);
    }
  }
}

Camera *Scene::getCamera() {
  // TODO: Add support for more cameras
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

Light *Scene::getLight() {
  // TODO: Add support for more lights
  for (auto *object : this->objects) {
    if (dynamic_cast<Light *>(object) == nullptr)
      continue;

    return static_cast<Light *>(object);
  }

  // no light found
  return nullptr;
}
