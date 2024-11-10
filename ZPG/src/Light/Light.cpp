#include "Light.hpp"

glm::vec3 Light::getColor() const { return this->color; }

void Light::assignId(int id) { this->id = id; }

int Light::getId() const { return this->id; }

float Light::getKc() const { return this->kc; }
float Light::getKl() const { return this->kl; }
float Light::getKq() const { return this->kq; }
