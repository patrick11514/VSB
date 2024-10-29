#pragma once

#include <functional>
#include <glm/ext/matrix_float4x4.hpp>
#include <memory>

#include "../Shader/ShaderProgram.hpp"
#include "../Transformation/Transformation.hpp"
#include "ObjectData.hpp"

// Define custom types
using RenderFunction =
    std::function<void(const glm::mat4x4 &transformationMatrix,
                       const ShaderProgram *shaderProgram)>;
using AnimationFunction = std::function<glm::mat4x4(
    const glm::mat4x4 &transformationMatrix, float elapsedTime)>;

class Scene;

/**
 * @brief This class put ObjectData, ShaderProgram, Transformations together
 * into single object
 */
class Object {
private:
  std::shared_ptr<ObjectData> data; ///< Sliced data of object
  ShaderProgram *shaderProgram;     ///< Shader program used on object
  std::shared_ptr<Transformation> transformations; ///< Object transformations
  RenderFunction
      renderFunction; ///< Function which will be called to render the object
  AnimationFunction
      animationFunction;    ///< Function which will be called to
                            ///< perform dynamic animations
                            ///< TODO: System which will automatically animate
                            ///< dynamic animations, if specified
  Scene *myScene = nullptr; ///< Scene in which is current object presented
public:
  /**
   * @brief Basic constructor used, when you don't want to animate object
   * @param data Sliced data of Object
   * @param shaderProgram Pointer to shader program, which will be used to
   * render this object
   * @param transformations Pointer to transformations used on this object
   * @param renderFunction Render function to render Object(s) from slided data
   */
  Object(std::shared_ptr<ObjectData> data, ShaderProgram *shaderProgram,
         std::shared_ptr<Transformation> transformations,
         RenderFunction renderFunction);
  /**
   * @brief Constructor used when you want to dynamically animate the object
   * @param data Sliced data of Object
   * @param shaderProgram Pointer to shader program, which will be used to
   * render this object
   * @param transformations Pointer to transformations used on this object
   * @param renderFunction Render function to render Object(s) from slided data
   * @param animationFunction Function used to animate dynamically object (eg.
   * spin it). Function will recieve glm::mat4 of currently applied animations
   * and float time (elapsed time of current animation) and you can perform some
   * transformation on current matrice and return new matrice
   * @note transformationFunction will likely change, because transformations
   * will support automatic transformations, and if transformation want to be
   * dynamic, it will automatically get the current time TODO: make it :)
   */

  Object(std::shared_ptr<ObjectData> data, ShaderProgram *shaderProgram,
         std::shared_ptr<Transformation> transformations,
         RenderFunction renderFunction, AnimationFunction animationFunction);
  /**
   * @brief Replace animation function
   * @param animationFunction Function used to animate dynamically object (eg.
   * spin it). Function will recieve glm::mat4 of currently applied animations
   * and float time (elapsed time of current animation) and you can perform some
   * transformation on current matrice and return new matrice
   * @note transformationFunction will likely change, because transformations
   * will support automatic transformations, and if transformation want to be
   * dynamic, it will automatically get the current time TODO: make it :)
   */
  void setAnimationFunction(AnimationFunction animationFunction);

  void assignScene(Scene *scene); ///< Assign scene to object automatically
                                  ///< called inside Scene::addObject

  /**
   * @brief Draw object using the renderFunction
   * @param time Elapsed time of animation
   */
  void draw(float time) const;
};
