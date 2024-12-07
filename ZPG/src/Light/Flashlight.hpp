#pragma once

/*
 * File: Flashlight.hpp
 * Author: Patrik Mintěl (MIN0150)
 * Description: File containing Flashlight class
 */

#include "../Camera.hpp"
#include "../Patterns/Observable.hpp"
#include "../Transformation/UpdatableTranslate.hpp"
#include "ReflectorLight.hpp"

#include <glm/ext/vector_float3.hpp>
#include <memory>

/**
 * @brief Anonymous namespace with FlashlightTransformation class, which is used
 * in Flashlight, since we need to pass transformation shared ptr to
 * ReflectorLight constructor, but  normal class atributes are initialized after
 * Base constructors, but because this is a class is used as first Base class,
 * the translate attribute is already initialized and can be used in other base
 * constructors
 */
namespace {
class FlashlightTransformation {
protected:
  UpdatableTranslate *translate;
  std::shared_ptr<Transformation> transformation =
      std::make_shared<Transformation>();
};
}; // namespace

/**
 * @brief Flashlight class which moves with paired camera
 */
class Flashlight : public FlashlightTransformation,
                   public ReflectorLight,
                   public Observer {
private:
public:
  /**
   * @brief Flashlight constructor
   * @param color Color of light
   * @param camera Camera paired with flashlight
   * @param kc Constant attenuation coeficient
   * @param kl Linera attenuation coeficient
   * @param kq Quadratic attenuation coeficient
   */
  Flashlight(glm::vec3 color, Camera *camera, float kc, float kl, float kq)
      : ReflectorLight(color, glm::vec3{1.0}, 20.f, this->transformation, kc,
                       kl, kq) {
    this->translate = new UpdatableTranslate(camera->getPosition());
    this->transformation->addTransformation(this->translate);

    camera->registerObserver(this);
  }

  void update(
      Observable *who) override; ///< Observer method which is called in Camera
};
