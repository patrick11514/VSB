#pragma once

#include "StaticScene.hpp"

/**
 * @brief Light scene, with some objects
 */
class DifferentLight : public StaticScene {
public:
  DifferentLight(const ShaderStorage &shaderStorage)
      : StaticScene(shaderStorage) {};
  void addObjects() override;
};
