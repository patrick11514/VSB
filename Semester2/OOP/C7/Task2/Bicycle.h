#ifndef __Bicycle_H_
#define __Bicycle_H_ 1

#include <string>
#include "Vehicle.h"

class Bicycle : public Vehicle
{
private:
    int gearCount;
    std::string type;

public:
    Bicycle(int speed, std::string color, std::string name, int gearCount, std::string type);

    std::string getType();
    int getGearCount();
};

#endif
