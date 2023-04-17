#include "Bicycle.h"
#include "Vehicle.h"

Bicycle::Bicycle(int speed, std::string color, std::string name, int gearCount, std::string type) : Vehicle(speed, color, 2, name)
{
    this->gearCount = gearCount;
    this->type = type;
}

std::string Bicycle::getType()
{
    return this->type;
}

int Bicycle::getGearCount()
{
    return this->gearCount;
}
