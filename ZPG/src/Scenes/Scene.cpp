#include "Scene.hpp"
#include "../Object/DrawableObject.hpp"

#include <GLFW/glfw3.h>
#include <glm/ext/matrix_float4x4.hpp>

void Scene::addObject(BaseObject object) {
  object.assignScene(this);
  this->objects.push_back(object);
}

void Scene::render() const {
  float time = glfwGetTime();

  for (const auto &object : this->objects) {
    const auto *objectPtr = &object;
    if (dynamic_cast<const DrawableObject *>(objectPtr) != nullptr) {
      static_cast<const DrawableObject *>(objectPtr)->draw(time);
    }
  }
}

Camera *Scene::getCamera() { return &this->camera; }
