#pragma once

#include "Observer.hpp"
#include <vector>

/*
 * @brief Object, which can be Observed with Observers
 */
class Observable {
private:
  std::vector<Observer *> observers; ///< Vector of observers

public:
  virtual ~Observable() = default;

  void registerObserver(Observer *); ///< Register new Observer
  void removeObserver(Observer *);   ///< Remove Observer
  void notifyObservers() const;      ///< Notify observers
};
