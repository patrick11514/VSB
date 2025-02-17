#pragma once

/*
 * File: Light.hpp
 * Author: Patrik Mintěl (MIN0150)
 * Description: File contains Light class
 */

#include "../Object/BaseObject.hpp"
#include "../Patterns/Observable.hpp"
#include "../Transformation/Transformation.hpp"

#include <glm/ext/vector_float3.hpp>

/**
 * @brief Type of light
 */
enum LightType { POINT, DIRECTIONAL, REFLECTOR };

/**
 * @brief Base Light class
 */
class Light : public BaseObject, public Observable {
private:
  std::shared_ptr<Transformation> transformations; ///< Object transformations

  LightType type;  ///< Type of light
  glm::vec3 color; ///< Color of light
  int id;          ///< Id of light
  float kc;        ///< contant attenuation
  float kl;        ///< linear attenuation
  float kq;        ///< quadratic attenuation
protected:
  /**
   * @brief This param says Shaderprogam if it should update light properties in
   * Shader
   *
   * This was implemented because, when Flashlight updated everytime with camera
   * move and it was constantly updating all light properties (which was
   * useless) and position, it lagged a bit, so Shader Program check this option
   * and if its true, then it update properties (kc, kq, kl, color)
   */
  bool propertiesUpdated = true;

public:
  /**
   * @brief Light constructor
   * @param color Color of light
   * @param transformations Transformations of light (position primarly)
   * @param kc Constant attenuation coeficient
   * @param kl Linera attenuation coeficient
   * @param kq Quadratic attenuation coeficient
   */
  Light(glm::vec3 color, std::shared_ptr<Transformation> transformations,
        float kc, float kl, float kq, LightType type)
      : transformations(transformations), type(type), color(color), kc(kc),
        kl(kl), kq(kq) {};

  virtual ~Light() = default; ///< Virtual destructor

  Transformation *getTransformations() const {
    return this->transformations.get();
  };

  void assignId(int id); ///< Assign id to this light

  int getId() const;

  glm::vec3 getColor() const;
  void setColor(glm::vec3 color);

  LightType getType() const;
  float getKc() const;
  float getKl() const;
  float getKq() const;
  bool getPropertiesUpdated(); ///< This method retrieve properties updated
                               ///< value and resets it to false, so next update
                               ///< doesn't update the properties again
};
