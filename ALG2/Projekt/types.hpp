#pragma once
#include <vector>

enum Face
{
    FRONT,
    BACK,
    LEFT,
    RIGHT,
    TOP,
    BOTTOM
};

enum Axis
{
    X,
    Y,
    Z,
    XInvert,
    YInvert,
    ZInvert
};

enum Status
{
    Unchecked,
    Checked
};

struct
{
    std::vector<Face> neighborsAtFaces;
    Face checkIfFaceHasNeighbor;
} typedef CubeScanData;

std::vector<Face> getFacesAtAxis(Axis axis);
CubeScanData getCubeDataAtAxis(Axis axis);
std::vector<Axis> getAllAxies();