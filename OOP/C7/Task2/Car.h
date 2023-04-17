#ifndef __Car_H_
#define __Car_H_ 1

#include <string>
#include "MotorVehicle.h"

class Car : public MotorVehicle
{
private:
    int doorCount;
    std::string wheelDrive;

public:
    Car(int speed, std::string color, std::string name, std::string motorVolume, std::string spz, std::string fuelType, std::string cylinderCount);

    int getDoorCount();
    std::string getWheelDrive();
};

#endif
