#pragma once

#include <entt/entt.hpp>

class ISystem
{
public:
    virtual ~ISystem() = default;
    virtual void update(entt::registry &registry) = 0;
};