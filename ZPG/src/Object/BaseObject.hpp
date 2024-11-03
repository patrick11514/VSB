#pragma once

class Scene;

class BaseObject {
private:
  Scene *myScene = nullptr; ///< Scene in which is current object presented
public:
  virtual ~BaseObject() = default;

  virtual void
  assignScene(Scene *scene); ///< Assign scene to object automatically
                             ///< called inside Scene::addObject
};
