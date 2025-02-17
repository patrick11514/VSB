#include "TextureStorage.hpp"
#include <stdexcept>

void TextureStorage::addTexture(const std::string &name,
                                std::shared_ptr<Texture> texture) {
  this->textures.emplace(name, texture);
}
void TextureStorage::removeTexture(const std::string &name) {
  auto it = this->textures.find(name);
  if (it == this->textures.end()) {
    return;
  }

  this->textures.erase(it);
}
std::shared_ptr<Texture>
TextureStorage::getTexture(const std::string &name) const {
  auto it = this->textures.find(name);
  if (it == this->textures.end()) {
    throw std::runtime_error("Unable to get texture");
  }

  return it->second;
}
