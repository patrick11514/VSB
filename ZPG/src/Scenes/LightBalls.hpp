#pragma once

#include "StaticScene.hpp"

/**
 * @brief Light scene, with some objects
 */
class LightBalls : public StaticScene {
public:
  LightBalls(const ShaderStorage &shaderStorage) : StaticScene(shaderStorage) {};
  void addObjects() override;
};
