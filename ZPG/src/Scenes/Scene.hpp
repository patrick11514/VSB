#pragma once

#include <vector>

#include "../Camera.hpp"
#include "../Object/Object.hpp"

/**
 * @brief Base scene class, which you can use to create scenes
 */
class Scene {
private:
  std::vector<Object> objects; ///< Vector of all objects in this scene
  Camera camera;

public:
  virtual ~Scene() = default;
  /**
   * @brief Add object to scene
   * @param object Object which will be added to scene
   */
  void addObject(Object object);

  Camera *getCamera(); ///< Get camera from scene

  void render() const; ///< Render scene called in mainLoop of application
};
