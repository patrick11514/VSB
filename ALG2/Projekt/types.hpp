#pragma once

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

Face getFaceAtAxis(Axis axis);
Axis getAxisAtFace(Face face);