#include "Vehicle.h"

Vehicle::Vehicle(int speed, std::string color, int wheelCount, std::string name)
{
    this->speed = speed;
    this->color = color;
    this->wheelCount = wheelCount;
    this->name = name;
}

int Vehicle::getSpeed()
{
    return this->speed;
}

std::string Vehicle::getColor()
{
    return this->color;
}

int Vehicle::getWheelCount()
{
    return this->wheelCount;
}

std::string Vehicle::getName()
{
    return this->name;
}
