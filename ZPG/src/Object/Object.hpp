#pragma once

#include <functional>
#include <glm/ext/matrix_float4x4.hpp>

#include "../Shader/ShaderProgram.hpp"
#include "ObjectData.hpp"
#include "Transformations.hpp"

class Object {
private:
  ObjectData data;
  const ShaderProgram &shaderProgram;
  Transformations transformations;
  std::function<void(const glm::mat4x4 &transformationMatrix,
                     const ShaderProgram &shaderProgram)>
      renderFunction;

public:
  Object(ObjectData data, const ShaderProgram &shaderProgram,
         Transformations transformations,
         std::function<void(const glm::mat4x4 &transformationMatrix,
                            const ShaderProgram &shaderProgram)>
             renderFunction);

  // set shader, set data and apply transformations
  void draw() const;
};
