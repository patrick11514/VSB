#include "ObjectDataStorage.hpp"
#include <memory>
#include <stdexcept>

bool ObjectDataStorage::hasObject(const std::string &name) const {
  auto it = this->objectDatas.find(name);
  return it != this->objectDatas.end();
}

std::shared_ptr<ObjectData>
ObjectDataStorage::getObject(const std::string &name) const {
  auto it = this->objectDatas.find(name);
  if (it == this->objectDatas.end()) {
    throw std::runtime_error("Unable to find object in storage");
  }
  return it->second;
}

void ObjectDataStorage::add(const std::string &name, ObjectData data) {
  this->objectDatas.emplace(name, std::make_shared<ObjectData>(data));
}
