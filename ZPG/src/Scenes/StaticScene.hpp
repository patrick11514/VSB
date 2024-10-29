#pragma once

#include "../Shader/ShaderStorage.hpp"
#include "Scene.hpp"

/**
 * @brief StaticScene, which createObjects inside it
 */
class StaticScene : public Scene {
protected:
  const ShaderStorage &shaderStorage;

public:
  StaticScene(const ShaderStorage &shaderStorage)
      : shaderStorage(shaderStorage) {
        }; ///< Constructor which takes static reference shaderStorage and
           ///< stores it inside self
  virtual ~StaticScene() = default; ///< Virtual destructor

  virtual void
  addObjects() = 0; ///< Function, which will be called in App::createObjects()
                    ///< to initialize objects in scene
};
