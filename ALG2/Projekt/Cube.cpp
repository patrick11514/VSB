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

bool Cube::isNeighbor(Cube *cube) const
{
    bool found = false;

    for (auto cube : this->neighbors)
    {
        if (cube->isSame(cube))
        {
            found = true;
            break;
        }
    }

    return found;
}

bool Cube::isNeighbor(Cube *cube, Face face) const
{
    return this->neighbors[face]->isSame(cube);
}

bool Cube::isSame(Cube *cube) const
{
    return cube->getX() == this->x && cube->getY() == this->y && cube->getZ() == this->z;
}

Cube *Cube::getCubeAt(Face face) const
{
    return this->neighbors[face];
}

std::vector<Cube *> Cube::getCubesAtAxis(Axis axis) const
{
    std::vector<Cube *> cubes;
    return std::vector<Cube *>();
}

void Cube::addNeighbor(Cube *cube, Face face)
{
    this->neighbors[face] = cube;
}