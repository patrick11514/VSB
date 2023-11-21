#include "Cube.hpp"
#include <iostream>
#include <queue>

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

    if (this->neighbors[data.checkIfFaceHasNeighbor] != nullptr)
    {
        return 0;
    }

    std::queue<Cube *> queue;

    queue.push(this);

    while (!queue.empty())
    {
        Cube *cube = queue.front();
        queue.pop();

        cube->status = Status::Checked;

        for (auto face : data.neighborsAtFaces)
        {
            Cube *cubeN = cube->neighbors[face];

            if (cubeN != nullptr && cubeN->getStatus() == Status::Unchecked)
            {
                queue.push(cubeN);
            }
        }
    }

    return 1;
}

std::vector<Cube *> Cube::getNeighborsAtAxis(Axis axis)
{
    std::vector<Face> faces = getFacesAtAxis(axis);

    std::vector<Cube *> cubes;
    std::queue<Cube *> queue;
    queue.push(this);

    while (!queue.empty())
    {
        Cube *cube = queue.front();
        queue.pop();

        cube->status = Status::Checked;

        cubes.push_back(cube);

        for (auto face : faces)
        {
            Cube *cubeN = cube->neighbors[face];

            if (cubeN != nullptr && cubeN->getStatus() == Status::Unchecked)
            {
                queue.push(cubeN);
            }
        }
    }
    return cubes;
}

void Cube::setStatus(Status status)
{
    this->status = status;
}
