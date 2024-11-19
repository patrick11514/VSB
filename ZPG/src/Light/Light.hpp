#pragma once

#include "../Object/BaseObject.hpp"
#include "../Patterns/Observable.hpp"
#include "../Transformation/Transformation.hpp"

#include <glm/ext/vector_float3.hpp>

enum LightType { POINT, DIRECTIONAL, REFLECTOR };
class Light : public BaseObject, public Observable {
private:
  std::shared_ptr<Transformation> transformations; ///< Object transformations

  LightType type;
  glm::vec3 color;
  int id;
  float kc; // contant attenuation
  float kl; // linear attenuation
  float kq; // quadratic attenuation
public:
  Light(glm::vec3 color, std::shared_ptr<Transformation> transformations,
        float kc, float kl, float kq, LightType type)
      : transformations(transformations), type(type), color(color), kc(kc),
        kl(kl), kq(kq) {};

  virtual ~Light() = default;

  Transformation *getTransformations() const {
    return this->transformations.get();
  };

  void assignId(int id);

  int getId() const;
  glm::vec3 getColor() const;
  LightType getType() const;
  float getKc() const;
  float getKl() const;
  float getKq() const;
};
