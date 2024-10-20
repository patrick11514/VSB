#pragma once

#include "../Shader/ShaderStorage.hpp"
#include "Scene.hpp"

/**
 * @brief First scene, with some objects
 */
class Objects : public Scene {
public:
  Objects(
      const ShaderStorage
          &shaderStorage); ///< Basic constructor, which will create some
                           ///< objects and store them to parent objects vector
};
