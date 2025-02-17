#pragma once

#include "StaticScene.hpp"

/**
 * @brief Forest Scene
 */
class DarkForest : public StaticScene {
public:
  DarkForest(const ShaderStorage &shaderStorage,
             const TextureStorage &textureStorage)
      : StaticScene(shaderStorage, textureStorage) {};
  void addObjects() override;
};
