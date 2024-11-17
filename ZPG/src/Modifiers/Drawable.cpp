#include "Drawable.hpp"

void Drawable::update() const {
  this->shaderProgram->putMaterial(this->material.get());
}

void Drawable::draw() const { this->update(); }
