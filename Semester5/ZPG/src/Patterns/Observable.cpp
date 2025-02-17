#include "Observable.hpp"
#include "Observer.hpp"

void Observable::registerObserver(Observer *observer) {
  this->observers.emplace_back(observer);
}

void Observable::removeObserver(Observer *observer) {
  for (auto it = this->observers.begin(); it != this->observers.end(); ++it) {
    if (*it == observer) {
      this->observers.erase(it);
      return;
    }
  }
}

void Observable::notifyObservers() {
  for (auto ptr : this->observers) {
    ptr->update(this);
  }
}
