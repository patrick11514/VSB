#include "Scene.hpp"
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

Camera *Scene::getCamera() { return &this->camera; }
