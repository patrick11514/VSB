#pragma once
#include <vector>
#include "types.hpp"

class Cube
{
private:
    std::vector<Cube *> neighbors;
    Status status = Status::Unchecked;
    int x;
    int y;
    int z;

public:
    Cube(int x, int y, int z);

    void addNeighbor(Cube *cube, Face face);

    int getX() const;
    int getY() const;
    int getZ() const;
    bool isNeighbor(Cube *cube) const;
    bool isNeighbor(Cube *cube, Face face) const;
    bool isSame(Cube *cube) const;
    Cube *getCubeAt(Face face) const;
    std::vector<Cube *> getCubesAtAxis(Axis axis) const;
};