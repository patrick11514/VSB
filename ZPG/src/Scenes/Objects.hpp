#pragma once

#include "StaticScene.hpp"

/**
 * @brief First scene, with some objects
 */
class Objects : public StaticScene {
public:
  Objects(const ShaderStorage &shaderStorage) : StaticScene(shaderStorage) {};
  void addObjects() override;
};
