#pragma once

#include "StaticScene.hpp"

/**
 * @brief Forest Scene
 */
class DarkForest : public StaticScene {
public:
  DarkForest(const ShaderStorage &shaderStorage)
      : StaticScene(shaderStorage) {};
  void addObjects() override;
};
