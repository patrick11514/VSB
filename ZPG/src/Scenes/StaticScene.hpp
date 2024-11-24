#pragma once

#include "../Object/Texture/TextureStorage.hpp"
#include "../Shader/ShaderStorage.hpp"
#include "Scene.hpp"

/**
 * @brief StaticScene, which createObjects inside it
 */
class StaticScene : public Scene {
protected:
  const ShaderStorage &shaderStorage;
  const TextureStorage &textureStorage;

public:
  StaticScene(const ShaderStorage &shaderStorage,
              const TextureStorage &textureStorage)
      : shaderStorage(shaderStorage), textureStorage(textureStorage) {
        }; ///< Constructor which takes static reference shaderStorage and
           ///< stores it inside self
  virtual ~StaticScene() = default; ///< Virtual destructor

  virtual void
  addObjects() = 0; ///< Function, which will be called in App::createObjects()
                    ///< to initialize objects in scene
};
