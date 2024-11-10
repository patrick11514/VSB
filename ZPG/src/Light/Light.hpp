#pragma once

#include "../Modifiers/Transformable.hpp"
#include "../Object/BaseObject.hpp"
#include "../Patterns/Observable.hpp"

#include <glm/ext/vector_float3.hpp>

class Light : public BaseObject, public Observable, public Transformable {
private:
  glm::vec3 color;
  float kc; // contant attenuation
  float kl; // linear attenuation
  float kq; // quadratic attenuation
  int id;

public:
  Light(glm::vec3 color, const std::shared_ptr<Transformation> &transformations,
        float kc, float kl, float kq)
      : Transformable(transformations), color(color), kc(kc), kl(kl), kq(kq) {};

  virtual ~Light() = default;

  void assignId(int id);

  int getId() const;
  glm::vec3 getColor() const;
  float getKc() const;
  float getKl() const;
  float getKq() const;
};
