#ifndef __Vehicle_H_
#define __Vehicle_H_ 1

#include <string>

class Vehicle
{
private:
    int speed;
    std::string color;
    int wheelCount;
    std::string name;

public:
    Vehicle(int speed, std::string color, int wheelCount, std::string name);

    int getSpeed();
    std::string getColor();
    int getWheelCount();
    std::string getName();
};

#endif
