#include "spherical_map.hpp"
#include <numbers>

SphericalMap::SphericalMap(const std::string &filename)
{
    texture_ = std::make_unique<Texture>(filename.c_str());
}

glm::vec3 SphericalMap::texel(const float x, const float y, const float z) const
{
    float O = std::atan2(y, x);
    float o = std::acos(z);
    float u = O / (2.f * std::numbers::pi) + 0.5f;
    float v = o / std::numbers::pi;

    auto color = texture_.get()->get_texel(u, v);
    return glm::vec3{color.r, color.g, color.b};
}