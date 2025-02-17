#include "RandomTranslate.hpp"
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>

glm::mat4 RandomTranslate::getMatrix() {
  float current = glfwGetTime();
  if (current - this->lastUpdate > 1) {
    this->xCurr = this->x(this->rng);
    this->yCurr = this->y(this->rng);
    this->zCurr = this->z(this->rng);
    this->lastUpdate = current;
  }

  this->finalMatrix =
      glm::translate(this->finalMatrix, glm::vec3{xCurr, yCurr, zCurr});

  return this->finalMatrix;
}
