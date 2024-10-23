#pragma once

#include "../Shader/ShaderStorage.hpp"
#include "Scene.hpp"

/**
 * @brief Light scene, with some objects
 */
class Light : public Scene {
public:
  Light(const ShaderStorage &
            shaderStorage); ///< Basic constructor, which will create some
                            ///< objects and store them to parent objects vector
};
