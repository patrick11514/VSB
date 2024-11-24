#pragma once

#include "../Patterns/Observable.hpp"
#include "../Shader/ShaderProgram.hpp"
#include "BaseObject.hpp"
#include "ObjectData.hpp"
#include "Texture/BaseTexture.hpp"
#include "Texture/Texture.hpp"

#include <array>
#include <glm/ext/vector_float3.hpp>
#include <memory>

class SkyBox : public BaseObject, public Observer, public BaseTexture {
private:
  glm::mat4 modelMatrix;
  shared_ptr<ObjectData> data;
  ShaderProgram *shaderProgram;

public:
  SkyBox(std::array<std::string, 6> faces, ShaderProgram *shaderProgram);

  void update(Observable *who);
  void assignScene(Scene *scene);
  void draw() const;
};
