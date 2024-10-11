#pragma once

#include <vector>

#include "../Object/Object.hpp"

class Scene
{
private:
    std::vector<Object> objects;

public:
    void addObject(Object object);

    void render() const;
};