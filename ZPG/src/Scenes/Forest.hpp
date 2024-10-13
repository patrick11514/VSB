#pragma once

#include <vector>

#include "../Object/Object.hpp"
#include "Scene.hpp"
#include "../Shader/ShaderStorage.hpp"

class Forest : public Scene
{
public:
    Forest(const ShaderStorage& shaderStorage);

    void render() const;
};
