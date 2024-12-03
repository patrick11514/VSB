#include "Drawable.hpp"

int Drawable::lastId = 1;

void Drawable::update() const {
  this->shaderProgram->putMaterial(this->material.get());
}

void Drawable::draw() {
  this->data->drawArrays();
  glStencilFunc(GL_ALWAYS, this->id, 0xFF);
}

void Drawable::assignId() {
  this->id = Drawable::lastId++;
  printf("Assigned %d\n", this->id);
}

int Drawable::getId() const { return this->id; }
