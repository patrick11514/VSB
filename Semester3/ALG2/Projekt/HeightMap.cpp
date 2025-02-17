#include "HeightMap.hpp"
#include <iostream>

Cube *HeightMap::getCube(int x, int y, int z) const
{
    if (x >= 1 && x <= this->width && y >= 1 && y <= this->length && z >= 1 && z <= this->height)
    {
        return this->cubes[this->getIndex(x, y, z)];
    }

    return nullptr;
}

void HeightMap::setCube(int x, int y, int z, Cube *cube)
{
    this->cubes[this->getIndex(x, y, z)] = cube;
}

int HeightMap::getMaxHeight(std::ifstream &file)
{
    int width, length, height;
    file >> length >> width;
    height = 0;

    int count = 0;
    while (!file.eof() && count < width * length)
    {
        int currentHeight;

        file >> currentHeight;
        if (currentHeight > height)
        {
            height = currentHeight;
        }
        count++;
    }

    return height;
}

void HeightMap::createCubes(std::ifstream &file)
{
    int x = 1, y = 1;
    int count = 0;

    while (!file.eof())
    {
        int currentHeight;
        file >> currentHeight;

        for (int z = 1; z <= currentHeight; ++z)
        {
            Cube *cube = new Cube(x, y, z);
            this->setCube(x, y, z, cube);
            count++;
        }

        x++;
        if (x > this->width)
        {
            x = 1;
            y++;
        }

        if (y > this->length)
        {
            break;
        }
    }

    std::cout << "Created " << count << " cubes" << std::endl;
}

void HeightMap::addNeighbors()
{
    // add neighbors
    for (auto &cube : this->cubes)
    {
        if (cube == nullptr)
        {
            continue;
        }

        // check faces, and we are looking from X axis (X goes to you, Y goes to right, Z goes up)
        int x = cube->getX();
        int y = cube->getY();
        int z = cube->getZ();

        // front
        Cube *front = this->getCube(x + 1, y, z);
        if (front != nullptr)
        {
            cube->addNeighbor(front, Face::FRONT);
        }

        // back
        Cube *back = this->getCube(x - 1, y, z);
        if (back != nullptr)
        {
            cube->addNeighbor(back, Face::BACK);
        }

        // left
        Cube *left = this->getCube(x, y - 1, z);
        if (left != nullptr)
        {
            cube->addNeighbor(left, Face::LEFT);
        }

        // right
        Cube *right = this->getCube(x, y + 1, z);
        if (right != nullptr)
        {
            cube->addNeighbor(right, Face::RIGHT);
        }

        // top
        Cube *top = this->getCube(x, y, z + 1);
        if (top != nullptr)
        {
            cube->addNeighbor(top, Face::TOP);
        }

        // bottom
        Cube *bottom = this->getCube(x, y, z - 1);
        if (bottom != nullptr)
        {
            cube->addNeighbor(bottom, Face::BOTTOM);
        }
    }
}

int HeightMap::getFacesAtAxis(Axis axis) const
{
    int faces = 0;

    std::vector<std::vector<Cube *>> parts;

    for (auto cube : this->cubes)
    {
        if (cube == nullptr || cube->getStatus() == Status::Checked)
        {
            continue;
        }

        std::vector<Cube *> cubes = cube->getNeighborsAtAxis(axis);

        parts.push_back(cubes);
    }

    this->resetStatus();

    // now check connected cubes
    for (auto cubes : parts)
    {
        for (auto cube : cubes)
        {
            if (cube->getStatus() == Status::Checked)
            {
                continue;
            }

            faces += cube->walkThroughNeighbors(axis);
        }
    }

    return faces;
}

void HeightMap::resetStatus() const
{
    for (auto &cube : this->cubes)
    {
        if (cube != nullptr)
        {
            cube->setStatus(Status::Unchecked);
        }
    }
}

int HeightMap::getIndex(int x, int y, int z) const
{
    return ((x - 1) * this->length * this->height) + ((y - 1) * this->height) + ((z - 1));
}

HeightMap::HeightMap(std::string fileName)
{
    // Check max dimensions
    std::ifstream file(fileName);
    if (!file.is_open())
    {
        std::cout << "Could not open file " << fileName << std::endl;
        return;
    }

    this->height = this->getMaxHeight(file);
    file.seekg(0);
    file >> this->length >> this->width;

    this->cubes = std::vector<Cube *>(width * length * height);
    this->createCubes(file);

    file.close();

    this->addNeighbors();
}

HeightMap::~HeightMap()
{
    int count = 0;
    for (auto &cube : this->cubes)
    {
        if (cube != nullptr)
        {
            count++;
            delete cube;
        }
    }

    std::cout << "Deleted " << count << " cubes" << std::endl;
}

int HeightMap::getFaces() const
{
    int faces = 0;

    std::vector<Axis> axies = getAllAxies();

    for (Axis axis : axies)
    {
        faces += this->getFacesAtAxis(axis);
        this->resetStatus();
    }

    return faces;
}
