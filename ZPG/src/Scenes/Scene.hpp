#pragma once

#include <vector>

#include "../Object/Object.hpp"

/**
 * @brief Base scene class, which you can use to create scenes
 */
class Scene {
private:
  std::vector<Object> objects; ///< Vector of all objects in this scene

public:
  /**
   * @brief Add object to scene
   * @param object Object which will be added to scene
   */
  void addObject(Object object);

  void render() const; ///< Render scene called in mainLoop of application
};
