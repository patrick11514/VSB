#pragma once
#include <string>
#include <fstream>
#include "Cube.hpp"

class HeightMap
{
private:
    std::vector<Cube *> cubes;
    int width;
    int length;
    int height;

    Cube *getCube(int x, int y, int z) const;
    void setCube(int x, int y, int z, Cube *cube);

    int getMaxHeight(std::ifstream &file);
    void createCubes(std::ifstream &file);
    void addNeighbors();

public:
    HeightMap(std::string fileName);
    ~HeightMap();

    int getFaces() const;
};