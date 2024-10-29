#pragma once

#include "StaticScene.hpp"

/**
 * @brief Forest Scene
 */
class Forest : public StaticScene {
public:
  Forest(const ShaderStorage &shaderStorage) : StaticScene(shaderStorage) {};
  void addObjects() override;
};
