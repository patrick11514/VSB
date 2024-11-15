#include "Flashlight.hpp"

void Flashlight::update(const Observable *who) {
  if (const auto *camera = dynamic_cast<const Camera *>(who)) {
    this->translate->update(camera->getPosition());
    this->direction = camera->getTarget();
    this->notifyObservers();
  }
}
