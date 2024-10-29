#pragma once

#include "StaticScene.hpp"

/**
 * @brief Light scene, with some objects
 */
class Light : public StaticScene {
public:
  Light(const ShaderStorage &shaderStorage) : StaticScene(shaderStorage) {};
  void addObjects() override;
};
