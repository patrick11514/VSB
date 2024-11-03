#pragma once

#include <vector>

#include "../Camera.hpp"
#include "../Object/BaseObject.hpp"

class Light;

/**
 * @brief Base scene class, which you can use to create scenes
 */
class Scene {
private:
  std::vector<BaseObject *> objects; ///< Vector of all objects in this scene

public:
  virtual ~Scene();
  /**
   * @brief Add object to scene
   * @param object Object which will be added to scene
   */
  void addObject(BaseObject *object);

  void addCamera(Camera *camera);

  Camera *
  getCamera(); ///< Get camera from scene TODO: add support for more cameras
  Light *getLight(); ///< Get light from scene TODO: add support for more light

  void render() const; ///< Render scene called in mainLoop of application
};
