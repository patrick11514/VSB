#pragma once

#include "StaticScene.hpp"

/**
 * @brief Forest Scene
 */
class Forest : public StaticScene {
public:
  Forest(const ShaderStorage &shaderStorage,
         const TextureStorage &textureStorage)
      : StaticScene(shaderStorage, textureStorage) {};
  void addObjects() override;
};
