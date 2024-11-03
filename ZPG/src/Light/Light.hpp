#pragma once

#include "../Object/BaseObject.hpp"
#include "../Patterns/Observable.hpp"

#include <glm/ext/vector_float3.hpp>

class Light : public BaseObject, public Observable {
private:
  glm::vec3 color;
  glm::vec3 position;

public:
  Light(glm::vec3 color, glm::vec3 position)
      : color(color), position(position) {};

  glm::vec3 getColor() const;
  glm::vec3 getPosition() const;
};
