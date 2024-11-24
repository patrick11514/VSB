#pragma once

#include "Texture.hpp"
#include <memory>
#include <unordered_map>
class TextureStorage {
private:
  std::unordered_map<std::string, std::shared_ptr<Texture>> textures;

public:
  void addTexture(const std::string &name, std::shared_ptr<Texture> texture);
  void removeTexture(const std::string &name);
  std::shared_ptr<Texture> getTexture(const std::string &name) const;
};
