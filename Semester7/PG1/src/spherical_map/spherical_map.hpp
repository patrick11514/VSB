#pragma once

#include <string>
#include <memory>
#include "../utils/structs.hpp"
#include "../texture/texture.hpp"
#include <glm/glm.hpp>

class SphericalMap
{
public:
    SphericalMap(const std::string &filename);
    glm::vec3 texel(const float x, const float y, const float z) const;

private:
    std::unique_ptr<Texture> texture_;
};