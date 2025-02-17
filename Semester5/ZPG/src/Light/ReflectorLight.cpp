#include "ReflectorLight.hpp"

glm::vec3 ReflectorLight::getDirection() const { return this->direction; }
float ReflectorLight::getAngle() const { return this->angle; }
