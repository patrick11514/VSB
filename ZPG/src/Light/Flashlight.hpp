#pragma once

#include "../Camera.hpp"
#include "../Patterns/Observable.hpp"
#include "../Transformation/UpdatableTranslate.hpp"
#include "ReflectorLight.hpp"

#include <glm/ext/vector_float3.hpp>
#include <memory>

namespace {
class FlashlightTransformation {
protected:
  UpdatableTranslate *translate;
  std::shared_ptr<Transformation> transformation =
      std::make_shared<Transformation>();
};
}; // namespace

class Flashlight : public FlashlightTransformation,
                   public ReflectorLight,
                   public Observer {
private:
public:
  Flashlight(glm::vec3 color, Camera *camera, float kc, float kl, float kq)
      : ReflectorLight(color, glm::vec3{1.0}, 15.f, this->transformation, kc,
                       kl, kq) {
    this->translate = new UpdatableTranslate(camera->getPosition());
    this->transformation->addTransformation(this->translate);

    camera->registerObserver(this);
  }

  void update(Observable *who) override;
};
