#pragma once

#include "StaticScene.hpp"

/**
 * @brief Light scene, with some objects
 */
class LightBalls : public StaticScene {
public:
  LightBalls(const ShaderStorage &shaderStorage,
             const TextureStorage &textureStorage)
      : StaticScene(shaderStorage, textureStorage) {};
  void addObjects() override;
};
