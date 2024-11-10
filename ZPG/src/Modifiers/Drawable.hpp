#pragma once

#include <functional>
#include <memory>

#include "../Object/ObjectData.hpp"
#include "../Shader/ShaderProgram.hpp"
#include "BasicAttribute.hpp"

// Define custom types
using RenderFunction = std::function<void()>;

class Drawable : public BasicAttribute {
protected:
  std::shared_ptr<ObjectData> data; ///< Sliced data of object
  ShaderProgram *shaderProgram
      [[maybe_unused]]; ///< Shader program used on object
  RenderFunction
      renderFunction; ///< Function which will be called to render the object

public:
  Drawable(std::shared_ptr<ObjectData> data, ShaderProgram *shaderProgram,
           RenderFunction renderFunction)
      : data(data), shaderProgram(shaderProgram),
        renderFunction(renderFunction) {};
  virtual ~Drawable() = default;

  virtual void draw() const = 0;
};
