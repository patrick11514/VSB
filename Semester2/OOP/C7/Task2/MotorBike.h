#ifndef __MotorBike_H_
#define __MotorBike_H_ 1

#include <string>
#include "MotorVehicle.h"

class MotorBike : public MotorVehicle
{
private:
    std::string type;

public:
    MotorBike(int speed, std::string color, std::string name, std::string motorVolume, std::string spz, std::string fuelType, std::string cylinderCount, std::string type);

    std::string getType();
};

#endif
