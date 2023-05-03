#include "Obdelnik.h"
#include "GeometricObject.h"

#include <iostream>

Obdelnik::Obdelnik(double a, double b)
{
    std::cout << "Obdelnik constructor" << std::endl;
    this->stranaA = a;
    this->stranaB = b;
}

Obdelnik::~Obdelnik()
{
    std::cout << "Obdelnik destructor" << std::endl;
}

double Obdelnik::VypocetObvodu()
{
    return this->stranaA * 2 + this->stranaB * 2;
}

double Obdelnik::VypocetObsahu()
{
    return this->stranaA * this->stranaB;
}
