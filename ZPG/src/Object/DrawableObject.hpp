#pragma once

#include <functional>
#include <glm/ext/matrix_float4x4.hpp>
#include <memory>

#include "../Shader/ShaderProgram.hpp"
#include "../Transformation/Transformation.hpp"
#include "BaseObject.hpp"
#include "ObjectData.hpp"

// Define custom types
using RenderFunction =
    std::function<void(const glm::mat4x4 &transformationMatrix,
                       const ShaderProgram *shaderProgram)>;
/**
 * @brief This class put ObjectData, ShaderProgram, Transformations together
 * into single object
 */
class DrawableObject : public BaseObject {
private:
  std::shared_ptr<ObjectData> data; ///< Sliced data of object
  ShaderProgram *shaderProgram;     ///< Shader program used on object
  std::shared_ptr<Transformation> transformations; ///< Object transformations
  RenderFunction
      renderFunction; ///< Function which will be called to render the object
public:
  /**
   * @brief Basic constructor used, when you don't want to animate object
   * @param data Sliced data of Object
   * @param shaderProgram Pointer to shader program, which will be used to
   * render this object
   * @param transformations Pointer to transformations used on this object
   * @param renderFunction Render function to render Object(s) from slided data
   */
  DrawableObject(std::shared_ptr<ObjectData> data, ShaderProgram *shaderProgram,
                 std::shared_ptr<Transformation> transformations,
                 RenderFunction renderFunction);

  /**
   * @brief Draw object using the renderFunction
   * @param time Elapsed time of animation
   */
  void draw(float time) const;
};
