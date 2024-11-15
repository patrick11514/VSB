#include "Light.hpp"

glm::vec3 Light::getColor() const { return this->color; }

void Light::assignId(int id) { this->id = id; }

int Light::getId() const { return this->id; }

LightType Light::getType() const { return this->type; }
