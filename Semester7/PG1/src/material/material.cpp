#include "material.hpp"
#include <cstring>

const char Material::kDiffuseMapSlot = 0;
const char Material::kSpecularMapSlot = 1;
const char Material::kNormalMapSlot = 2;
const char Material::kOpacityMapSlot = 3;

Material::Material() {
  // defaultn� materi�l
  ambient = glm::vec3(0.1f, 0.1f, 0.1f);
  diffuse = glm::vec3(0.4f, 0.4f, 0.4f);
  specular = glm::vec3(0.8f, 0.8f, 0.8f);

  emission = glm::vec3(0.0f, 0.0f, 0.0f);

  reflectivity = static_cast<float>(0.99);
  shininess = 1;

  ior = -1;

  memset(textures_, 0, sizeof(*textures_) * NO_TEXTURES);

  name_ = "default";
}

Material::Material(std::string &name, const glm::vec3 &ambient,
                   const glm::vec3 &diffuse, const glm::vec3 &specular,
                   const glm::vec3 &emission, const float reflectivity,
                   const float shininess, const float ior, Texture **textures,
                   const int no_textures) {
  name_ = name;

  this->ambient = ambient;
  this->diffuse = diffuse;
  this->specular = specular;

  this->emission = emission;

  this->reflectivity = reflectivity;
  this->shininess = shininess;

  this->ior = ior;

  this->shader = Shader::BASIC;

  if (textures) {
    memcpy(textures_, textures, sizeof(textures) * no_textures);
  }
}

Material::~Material() {
  for (int i = 0; i < NO_TEXTURES; ++i) {
    if (textures_[i]) {
      delete[] textures_[i];
      textures_[i] = nullptr;
    };
  }
}

void Material::set_name(const char *name) { name_ = std::string(name); }

std::string Material::get_name() const { return name_; }

void Material::set_texture(const int slot, Texture *texture) {
  textures_[slot] = texture;
}

Texture *Material::get_texture(const int slot) const { return textures_[slot]; }