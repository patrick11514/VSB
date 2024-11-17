#pragma once

#include <functional>
#include <memory>

#include "../Object/Material.hpp"
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
  std::shared_ptr<Material> material;

public:
  Drawable(std::shared_ptr<ObjectData> data, ShaderProgram *shaderProgram,
           RenderFunction renderFunction, std::shared_ptr<Material> material)
      : data(data), shaderProgram(shaderProgram),
        renderFunction(renderFunction), material(material) {};
  virtual ~Drawable() = default;

  virtual void draw() const = 0;
};
