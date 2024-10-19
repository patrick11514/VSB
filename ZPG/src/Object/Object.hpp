#pragma once

#include <functional>
#include <glm/ext/matrix_float4x4.hpp>

#include "../Shader/ShaderProgram.hpp"
#include "ObjectData.hpp"
#include "Transformations.hpp"

using RenderFunction =
    std::function<void(const glm::mat4x4 &transformationMatrix,
                       const ShaderProgram *shaderProgram)>;
using AnimationFunction = std::function<glm::mat4x4(
    const glm::mat4x4 &transformationMatrix, float elapsedTime)>;

class Object {
private:
  ObjectData data;
  const ShaderProgram *shaderProgram;
  Transformations transformations;
  RenderFunction renderFunction;
  AnimationFunction animationFunction;

public:
  Object(ObjectData data, const ShaderProgram *shaderProgram,
         Transformations transformations, RenderFunction renderFunction);

  Object(ObjectData data, const ShaderProgram *shaderProgram,
         Transformations transformations, RenderFunction renderFunction,
         AnimationFunction function);

  void setAnimationFunction(AnimationFunction function);

  // set shader, set data and apply transformations
  void draw(float time) const;
};
