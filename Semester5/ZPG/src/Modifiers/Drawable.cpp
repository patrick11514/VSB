#include "Drawable.hpp"

int Drawable::lastId = 1;

void Drawable::update() const {
  this->shaderProgram->putMaterial(this->material.get());
}

void Drawable::draw() {
  glStencilFunc(GL_ALWAYS, this->id, 0xFF);
  this->data->drawArrays();
}

void Drawable::assignId() {
  this->id = Drawable::lastId++;
  printf("Assigned %d\n", this->id);
}

int Drawable::getId() const { return this->id; }
