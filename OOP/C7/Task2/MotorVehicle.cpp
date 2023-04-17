#include "MotorVehicle.h"
#include "Vehicle.h"

MotorVehicle::MotorVehicle(int speed, std::string color, int wheelCount, std::string name, std::string motorVolume, std::string spz, std::string fuelType, std::string cylinderCount) : Vehicle(speed, color, wheelCount, name)
{
    this->motorVolume = motorVolume;
    this->spz = spz;
    this->fuelType = fuelType;
    this->cylinderCount = cylinderCount;
}

std::string MotorVehicle::getMotorVolume()
{
    return this->motorVolume;
}

std::string MotorVehicle::getSpz()
{
    return this->spz;
}

std::string MotorVehicle::getFuelType()
{
    return this->fuelType;
}

std::string MotorVehicle::getCylinderCount()
{
    return this->cylinderCount;
}
