#include "Scene.hpp"

void Scene::addObject(Object object)
{
    this->objects.push_back(object);
}

void Scene::render() const
{
    for (const auto &object : this->objects)
    {
        object.draw();
    }
}
