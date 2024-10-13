#include "Scene.hpp"
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_float4x4.hpp>

void Scene::addObject(Object object)
{
    this->objects.push_back(object);
}

void Scene::render() const
{
    float time = glfwGetTime();

    for (const auto &object : this->objects)
    {
        object.draw(time);
    }
}
