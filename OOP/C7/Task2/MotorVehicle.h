#ifndef __MotorVehicle_H_
#define __MotorVehicle_H_ 1

#include <string>
#include "Vehicle.h"

class MotorVehicle : public Vehicle
{
private:
    std::string motorVolume;
    std::string spz;
    std::string fuelType;
    std::string cylinderCount;

public:
    MotorVehicle(int speed, std::string color, int wheelCount, std::string name, std::string motorVolume, std::string spz, std::string fuelType, std::string cylinderCount);

    std::string getMotorVolume();
    std::string getSpz();
    std::string getFuelType();
    std::string getCylinderCount();
};

#endif
