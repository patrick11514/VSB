#include "LineMovement.hpp"
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>

glm::mat4 LineMovement::getMatrix() {
  float time = glfwGetTime();
  float frameTime = std::fmod(time, (this->moveDuration * 2));
  float percentage;
  if (frameTime <= moveDuration) {
    percentage = frameTime / this->moveDuration;
  } else {
    percentage = (this->moveDuration * 2 - frameTime) / this->moveDuration;
  }
  return glm::translate(glm::mat4{1},
                        this->A + (this->B - this->A) * percentage);
}
