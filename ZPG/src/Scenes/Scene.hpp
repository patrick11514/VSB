#pragma once

#include <vector>

#include "../Camera.hpp"
#include "../Object/BaseObject.hpp"

/**
 * @brief Base scene class, which you can use to create scenes
 */
class Scene {
private:
  std::vector<BaseObject> objects; ///< Vector of all objects in this scene
  Camera camera;

public:
  virtual ~Scene() = default;
  /**
   * @brief Add object to scene
   * @param object Object which will be added to scene
   */
  void addObject(BaseObject object);

  Camera *getCamera(); ///< Get camera from scene

  void render() const; ///< Render scene called in mainLoop of application
};
