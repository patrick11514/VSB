#include "DynamicRotation.hpp"
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>

glm::mat4 DynamicRotation::getMatrix() const {
  return glm::rotate(glm::mat4{1},
                     this->angle * (float)glfwGetTime() * this->timeFactor,
                     this->axes);
}
