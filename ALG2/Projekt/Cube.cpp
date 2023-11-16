#include "Cube.hpp"
#include <iostream>

Cube::Cube(int x, int y, int z)
{
    this->neighbors = std::vector<Cube *>(6);
    this->x = x;
    this->y = y;
    this->z = z;
}

int Cube::getX() const
{
    return this->x;
}

int Cube::getY() const
{
    return this->y;
}

int Cube::getZ() const
{
    return this->z;
}

void Cube::addNeighbor(Cube *cube, Face face)
{
    this->neighbors[face] = cube;
}