#pragma once
#include "types.hpp"
#include <string>

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
    int walkThroughNeighbors(Axis axis);
    std::vector<Cube *> getNeighborsAtAxis(Axis axis);
    void setStatus(Status status);

    int getX() const;
    int getY() const;
    int getZ() const;
    bool isNeighbor(Cube *cube) const;
    bool isNeighbor(Cube *cube, Face face) const;
    bool isSame(Cube *cube) const;
    Cube *getCubeAt(Face face) const;
    Status getStatus() const;
    std::string toString() const;
};