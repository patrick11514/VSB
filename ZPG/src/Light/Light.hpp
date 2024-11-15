#pragma once

#include "../Modifiers/Transformable.hpp"
#include "../Object/BaseObject.hpp"
#include "../Patterns/Observable.hpp"

#include <glm/ext/vector_float3.hpp>

enum LightType { POINT, DIRECTIONAL, REFLECTOR };
class Light : public BaseObject, public Observable, public Transformable {
private:
  LightType type;
  glm::vec3 color;
  int id;

public:
  Light(glm::vec3 color, std::shared_ptr<Transformation> transformations,
        LightType type)
      : Transformable(transformations), type(type), color(color) {};

  virtual ~Light() = default;

  void assignId(int id);

  int getId() const;
  glm::vec3 getColor() const;
  LightType getType() const;
};
