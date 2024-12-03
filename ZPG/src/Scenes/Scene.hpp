#pragma once

#include <unordered_set>
#include <vector>

#include "../Camera.hpp"
#include "../Modifiers/Drawable.hpp"
#include "../Object/BaseObject.hpp"
#include "../Object/SkyBox.hpp"
#include "../Shader/ShaderProgram.hpp"

class Light;

/**
 * @brief Base scene class, which you can use to create scenes
 */
class Scene {
private:
  std::vector<BaseObject *> objects; ///< Vector of all objects in this scene
  int lightCount = 0;
  std::unordered_set<ShaderProgram *>
      usedPrograms; ///< Set of used shader programs

public:
  virtual ~Scene();
  /**
   * @brief Add object to scene
   * @param object Object which will be added to scene
   */
  void addObject(BaseObject *object);
  void removeObject(BaseObject *object);

  void addCamera(Camera *camera);

  Camera *getCamera(); ///< Get camera from scene

  std::vector<Camera *> getCameras();

  std::vector<Light *> getLights() const; ///< Get light from scene

  SkyBox *getSkybox() const; ///< Get skybox from scene

  Light *getLight(int id) const;

  void activate(); ///< Put all things to shaders ready for render

  void registerProgram(ShaderProgram *program);

  void render(); ///< Render scene called in mainLoop of application

  Drawable *getObject(int id);
};
