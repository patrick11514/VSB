#pragma once

/*
 * File: Drawable.hpp
 * Author: Patrik Mintěl (MIN0150)
 * Description: File contains Drawable attribute
 */

#include <functional>
#include <memory>

#include "../Object/Material/Material.hpp"
#include "../Object/ObjectData.hpp"
#include "../Shader/ShaderProgram.hpp"
#include "BasicAttribute.hpp"

// Define custom types
using RenderFunction = std::function<void()>;

/**
 * @brief Drawable attribute
 */
class Drawable : public BasicAttribute {
private:
  static int lastId; // Id of last drawable, so we can assign ids

protected:
  std::shared_ptr<ObjectData> data; ///< Sliced data of object
  ShaderProgram *shaderProgram
      [[maybe_unused]];          ///< Shader program used on object
  RenderFunction renderFunction; ///< Function which will be called to render
                                 ///< the object if provided
  std::shared_ptr<Material> material; ///< Material of drawable
  int id = -1;                        ///< Id of drawable, if assigned

public:
  /**
   * @brief Drawable constructor
   * @param data Data of the object (points, normals, uv...),
   * @param shaderProgram Shader Program used to render Drawable
   * @param material Material (or Texture) used on Drawable
   * @param renderFunction Render Function which is optional and is used
   * specific cases, like draw more things from data (3 triangles, 2 quads
   * etc..), because in default it draw everything as triangles
   */
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
