#pragma once

#include "../Modifiers/BasicAttribute.hpp"

class Scene;

class BaseObject : public BasicAttribute {
private:
  Scene *myScene = nullptr; ///< Scene in which is current object presented
public:
  virtual ~BaseObject() = default;

  virtual void
  assignScene(Scene *scene); ///< Assign scene to object automatically
                             ///< called inside Scene::addObject
};
