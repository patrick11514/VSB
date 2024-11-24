#pragma once

#include "StaticScene.hpp"

/**
 * @brief Light scene, with some objects
 */
class DifferentLight : public StaticScene {
public:
  DifferentLight(const ShaderStorage &shaderStorage,
                 const TextureStorage &textureStorage)
      : StaticScene(shaderStorage, textureStorage) {};
  void addObjects() override;
};
