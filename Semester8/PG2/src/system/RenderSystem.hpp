#pragma once
#include "ISystem.hpp"

class Rasterizer;

class RenderSystem : public ISystem
{
private:
    Rasterizer *rasterizer;

public:
    RenderSystem(Rasterizer *rast) : rasterizer(rast) {}
    void update(entt::registry &registry) override;
};