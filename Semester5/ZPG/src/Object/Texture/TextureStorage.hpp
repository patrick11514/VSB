#pragma once

/*
 * File: TextureStorage.hpp
 * Author: Patrik Mintěl (MIN0150)
 * Description: File contains Texture Storage for storing textures
 */

#include "Texture.hpp"
#include <memory>
#include <unordered_map>

/**
 * @brief Texture Strorage class
 */
class TextureStorage {
private:
  std::unordered_map<std::string, std::shared_ptr<Texture>>
      textures; ///< Here is stored all textures by string key

public:
  void addTexture(const std::string &name,
                  std::shared_ptr<Texture> texture); ///< Add texture to stroage
  void removeTexture(
      const std::string &name); ///< Remove texture from storage by key
  std::shared_ptr<Texture> getTexture(const std::string &name)
      const; ///< Get texture from key, Throws, when invalid key
};
