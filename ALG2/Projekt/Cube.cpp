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

Status Cube::getStatus() const
{
    return this->status;
}

std::string Cube::toString() const
{
    return "Cube at x:" + std::to_string(this->x) + ", y:" + std::to_string(this->y) + ", z:" + std::to_string(this->z);
}

void Cube::addNeighbor(Cube *cube, Face face)
{
    this->neighbors[face] = cube;
}

int Cube::walkThroughNeighbors(Axis axis)
{
    CubeScanData data = getCubeDataAtAxis(axis);

    this->status = Status::Checked;

    if (this->neighbors[data.checkIfFaceHasNeighbor] != nullptr)
    {
        return 0;
    }

    for (auto face : data.neighborsAtFaces)
    {
        Cube *cube = this->neighbors[face];

        if (cube != nullptr && cube->getStatus() == Status::Unchecked)
        {
            cube->walkThroughNeighbors(axis);
        }
    }

    return 1;
}

std::vector<Cube *> Cube::getNeighborsAtAxis(Axis axis)
{
    std::vector<Face> faces = getFacesAtAxis(axis);

    this->status = Status::Checked;

    std::vector<Cube *> cubes;
    cubes.push_back(this);

    for (auto face : faces)
    {
        Cube *cube = this->neighbors[face];

        if (cube != nullptr && cube->getStatus() == Status::Unchecked)
        {
            std::vector<Cube *> otherCubes = cube->getNeighborsAtAxis(axis);

            cubes.insert(cubes.end(), otherCubes.begin(), otherCubes.end());
        }
    }

    return cubes;
}

void Cube::setStatus(Status status)
{
    this->status = status;
}
