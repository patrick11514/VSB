#pragma once

#include <functional>
#include <memory>

#include "../Object/Material/Material.hpp"
#include "../Object/ObjectData.hpp"
#include "../Object/Texture/Texture.hpp"
#include "../Shader/ShaderProgram.hpp"
#include "BasicAttribute.hpp"

// Define custom types
using RenderFunction = std::function<void()>;

enum TextureType { MATERIAL, TEXTURE };

class Drawable : public BasicAttribute {
protected:
  std::shared_ptr<ObjectData> data; ///< Sliced data of object
  ShaderProgram *shaderProgram
      [[maybe_unused]]; ///< Shader program used on object
  RenderFunction
      renderFunction; ///< Function which will be called to render the object

  TextureType textureType;

  std::shared_ptr<Material> material;
  std::shared_ptr<Texture> texture;

public:
  Drawable(std::shared_ptr<ObjectData> data, ShaderProgram *shaderProgram,
           RenderFunction renderFunction, std::shared_ptr<Material> material)
      : data(data), shaderProgram(shaderProgram),
        renderFunction(renderFunction), textureType(TextureType::MATERIAL),
        material(material) {};

  Drawable(std::shared_ptr<ObjectData> data, ShaderProgram *shaderProgram,
           RenderFunction renderFunction, std::shared_ptr<Texture> texture)
      : data(data), shaderProgram(shaderProgram),
        renderFunction(renderFunction), textureType(TextureType::TEXTURE),
        texture(texture) {};

  virtual ~Drawable() = default;

  virtual void update() const;
  virtual void draw();
};
