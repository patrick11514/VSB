#pragma once

#include "StaticScene.hpp"

/**
 * @brief First scene, with some objects
 */
class Objects : public StaticScene {
public:
  Objects(const ShaderStorage &shaderStorage,
          const TextureStorage &textureStorage)
      : StaticScene(shaderStorage, textureStorage) {};
  void addObjects() override;
};
