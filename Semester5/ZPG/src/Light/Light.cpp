#include "Light.hpp"

void Light::setColor(glm::vec3 color) {
  this->propertiesUpdated = true;
  this->color = color;
  this->notifyObservers();
}
glm::vec3 Light::getColor() const { return this->color; }

void Light::assignId(int id) { this->id = id; }

int Light::getId() const { return this->id; }

LightType Light::getType() const { return this->type; }

float Light::getKc() const { return this->kc; }
float Light::getKl() const { return this->kl; }
float Light::getKq() const { return this->kq; }
bool Light::getPropertiesUpdated() {
  bool curr = this->propertiesUpdated;
  this->propertiesUpdated = false;
  return curr;
}
