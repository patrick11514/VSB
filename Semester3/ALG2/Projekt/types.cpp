#include "types.hpp"

std::vector<Face> getFacesAtAxis(Axis axis)
{
    std::vector<Face> faces;

    if (axis == Axis::X || axis == Axis::XInvert)
    {
        faces.push_back(Face::LEFT);
        faces.push_back(Face::RIGHT);
        faces.push_back(Face::TOP);
        faces.push_back(Face::BOTTOM);
    }
    else if (axis == Axis::Y || axis == Axis::YInvert)
    {
        faces.push_back(Face::FRONT);
        faces.push_back(Face::BACK);
        faces.push_back(Face::TOP);
        faces.push_back(Face::BOTTOM);
    }
    else if (axis == Axis::Z || axis == Axis::ZInvert)
    {
        faces.push_back(Face::FRONT);
        faces.push_back(Face::BACK);
        faces.push_back(Face::LEFT);
        faces.push_back(Face::RIGHT);
    }

    return faces;
}

CubeScanData getCubeDataAtAxis(Axis axis)
{

    CubeScanData data;

    if (axis == Axis::X || axis == Axis::XInvert)
    {
        data.checkIfFaceHasNeighbor = axis == Axis::X ? Face::FRONT : Face::BACK;
    }
    else if (axis == Axis::Y || axis == Axis::YInvert)
    {
        data.checkIfFaceHasNeighbor = axis == Axis::Y ? Face::LEFT : Face::RIGHT;
    }
    else if (axis == Axis::Z || axis == Axis::ZInvert)
    {
        data.checkIfFaceHasNeighbor = axis == Axis::Z ? Face::BOTTOM : Face::TOP;
    }

    data.neighborsAtFaces = getFacesAtAxis(axis);

    return data;
}

std::vector<Axis> getAllAxies()
{
    std::vector<Axis> axies;
    axies.push_back(Axis::X);
    axies.push_back(Axis::XInvert);
    axies.push_back(Axis::Y);
    axies.push_back(Axis::YInvert);
    axies.push_back(Axis::Z);
    axies.push_back(Axis::ZInvert);
    return axies;
}
