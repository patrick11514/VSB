#pragma once

#include "../Shader/ShaderStorage.hpp"
#include "Scene.hpp"

/**
 * @brief Forest Scene
 */
class Forest : public Scene {
public:
  Forest(const ShaderStorage
             &shaderStorage); ///< Constructor, which will create the scene and
                              ///< store object to parent Objects vector
};
