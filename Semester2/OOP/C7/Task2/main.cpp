#include <iostream>
#include <string>

#include "Bicycle.h"
#include "MotorBike.h"
#include "Car.h"

int main()
{
    Bicycle *kolo = new Bicycle(10, "cervena", "Blesk", 4, "Turbo 4");

    std::cout << "Kolo " << kolo->getName() << " ma " << kolo->getGearCount() << " rychlosti a je barvy " << kolo->getColor() << std::endl;

    delete kolo;

    MotorBike *motorka = new MotorBike(100, "modra", "Honda", "1000", "4A2 3000", "benzin", "4", "sport");

    std::cout << "Motorka " << motorka->getName() << " ma " << motorka->getMotorVolume() << " motor a je barvy " << motorka->getColor() << std::endl;

    delete motorka;

    Car *auticko = new Car(140, "zelena", "Skoda", "800", "4A2 3000", "benzin", "4");

    std::cout << "Auticko " << auticko->getName() << " ma " << auticko->getMotorVolume() << " motor a je barvy " << auticko->getColor() << std::endl;

    delete auticko;

    return 0;
}