#include "Car.h"
#include "MotorVehicle.h"

Car::Car(int speed, std::string color, std::string name, std::string motorVolume, std::string spz, std::string fuelType, std::string cylinderCount) : MotorVehicle(speed, color, 4, name, motorVolume, spz, fuelType, cylinderCount)
{
    this->doorCount = doorCount;
    this->wheelDrive = wheelDrive;
}

int Car::getDoorCount()
{
    return this->doorCount;
}

std::string Car::getWheelDrive()
{
    return this->wheelDrive;
}