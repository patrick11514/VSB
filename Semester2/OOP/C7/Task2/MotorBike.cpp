#include "MotorBike.h"
#include "MotorVehicle.h"

MotorBike::MotorBike(int speed, std::string color, std::string name, std::string motorVolume, std::string spz, std::string fuelType, std::string cylinderCount, std::string type) : MotorVehicle(speed, color, 2, name, motorVolume, spz, fuelType, cylinderCount)
{
    this->type = type;
}

std::string MotorBike::getType()
{
    return this->type;
}
