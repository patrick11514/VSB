#pragma once
#include <vector>
#include "types.hpp"

class Cube
{
private:
    std::vector<Cube *> neighbors;
    int x;
    int y;
    int z;

public:
    Cube(int x, int y, int z);

    void addNeighbor(Cube *cube, Face face);

    int getX() const;
    int getY() const;
    int getZ() const;
};