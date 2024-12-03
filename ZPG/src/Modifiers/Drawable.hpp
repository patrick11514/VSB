#pragma once

#include <functional>
#include <memory>

#include "../Object/Material/Material.hpp"
#include "../Object/ObjectData.hpp"
#include "../Shader/ShaderProgram.hpp"
#include "BasicAttribute.hpp"

// Define custom types
using RenderFunction = std::function<void()>;

class Drawable : public BasicAttribute {
private:
  static int lastId;

protected:
  std::shared_ptr<ObjectData> data; ///< Sliced data of object
  ShaderProgram *shaderProgram
      [[maybe_unused]]; ///< Shader program used on object
  RenderFunction
      renderFunction; ///< Function which will be called to render the object

  std::shared_ptr<Material> material;

  int id = -1;

public:
  Drawable(std::shared_ptr<ObjectData> data, ShaderProgram *shaderProgram,
           std::shared_ptr<Material> material,
           RenderFunction renderFunction = nullptr)
      : data(data), shaderProgram(shaderProgram),
        renderFunction(renderFunction), material(material) {};

  virtual ~Drawable() = default;

  void assignId();
  int getId() const;

  virtual void update() const;
  virtual void draw();
};
