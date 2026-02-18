#pragma once

/*
 * File: BaseObject.hpp
 * Author: Patrik Mintěl (MIN0150)
 * Description: File contains BaseObject class
 */

#include "../Modifiers/BasicAttribute.hpp"

// forward declaration
class Scene;

/**
 * @brief BaseObject, which is just everything in our Scene
 */
class BaseObject : public BasicAttribute {
private:
  Scene *myScene = nullptr; ///< Scene in which is current object presented
public:
  virtual ~BaseObject() = default;

  virtual void
  assignScene(Scene *scene); ///< Assign scene to object automatically
                             ///< called inside Scene::addObject
};
