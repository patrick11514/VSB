#pragma once

#include <memory>

#include "../Transformation/Transformation.hpp"
#include "BasicAttribute.hpp"

class Transformable : public BasicAttribute {
private:
  std::shared_ptr<Transformation> transformations; ///< Object transformations
public:
  Transformable(std::shared_ptr<Transformation> transformations)
      : transformations(transformations) {};

  virtual ~Transformable() = default;

  const Transformation *getTransformations() const {
    return this->transformations.get();
  };
};
