#include "types.hpp"

Face getFaceAtAxis(Axis axis)
{
    if (axis == Axis::X)
    {
        return Face::FRONT;
    }
    else if (axis == Axis::XInvert)
    {
        return Face::BACK;
    }
    else if (axis == Axis::Y)
    {
        return Face::RIGHT;
    }
    else if (axis == Axis::YInvert)
    {
        return Face::LEFT;
    }
    else if (axis == Axis::Z)
    {
        return Face::TOP;
    }
    else if (axis == Axis::ZInvert)
    {
        return Face::BOTTOM;
    }
}

Axis getAxisAtFace(Face face)
{
    if (face == Face::FRONT)
    {
        return Axis::X;
    }
    else if (face == Face::BACK)
    {
        return Axis::XInvert;
    }
    else if (face == Face::RIGHT)
    {
        return Axis::Y;
    }
    else if (face == Face::LEFT)
    {
        return Axis::YInvert;
    }
    else if (face == Face::TOP)
    {
        return Axis::Z;
    }
    else if (face == Face::BOTTOM)
    {
        return Axis::ZInvert;
    }
}