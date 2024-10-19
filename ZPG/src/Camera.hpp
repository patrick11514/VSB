#pragma once

#include <glm/ext/matrix_transform.hpp>
#include <vector>

#include "Patterns/Observer.hpp"

class Camera {
private:
  std::vector<Observer *> observers;

  glm::vec3 eye{0, 0, 0};
  glm::vec3 target{0};
  glm::vec3 up{0, -1, 0};

  float pitch = 0;
  float yaw = 0;

  void recalculateTarget();

public:
  Camera();

  void addObserver(Observer *observer);
  void removeObserver(Observer *observer);

  void broadcast() const;

  void toLeft();
  void toRight();
  void forward();
  void backward();
  void changeYaw(float deg);
  void changePitch(float deg);

  glm::mat4 calculateViewMatrix() const;
};
