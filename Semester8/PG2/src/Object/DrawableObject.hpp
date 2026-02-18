#pragma once

/*
 * File: DrawableObject.hpp
 * Author: Patrik Mintěl (MIN0150)
 * Description: File contains DrawableObject class
 */

#include <glm/ext/matrix_float4x4.hpp>
#include <memory>

#include "../Modifiers/Drawable.hpp"
#include "../Shader/ShaderProgram.hpp"
#include "../Transformation/Transformation.hpp"
#include "BaseObject.hpp"
#include "ObjectData.hpp"

/**
 * @brief This class put ObjectData, ShaderProgram, Transformations together
 * into single object
 */
class DrawableObject : public BaseObject, public Drawable {
private:
  std::shared_ptr<Transformation> transformations; ///< Object transformations
public:
  /**
   * @brief Basic constructor used, when you don't want to animate object
   * @param data Sliced data of Object
   * @param shaderProgram Pointer to shader program, which will be used to
   * render this object
   * @param transformations Pointer to transformations used on this object
   * @param renderFunction Render Function which is optional and is used
   * specific cases, like draw more things from data (3 triangles, 2 quads
   * etc..), because in default it draw everything as triangles
   */
  DrawableObject(std::shared_ptr<ObjectData> data, ShaderProgram *shaderProgram,
                 std::shared_ptr<Transformation> transformations,
                 std::shared_ptr<Material> material,
                 RenderFunction renderFunction = nullptr)
      : Drawable(data, shaderProgram, material, renderFunction),
        transformations(transformations) {};

  void
  assignScene(Scene *scene) override; ///< Assign scene to object automatically
                                      ///< called inside Scene::addObject

  Transformation *getTransformations() const {
    return this->transformations.get();
  };

  void draw() override;
};
