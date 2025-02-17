#include "Ctverec.h"

#include <iostream>

Ctverec::Ctverec(double s) : GeometricObject()
{
    std::cout << "Ctverec constructor" << std::endl;
    this->strana = s;
}

Ctverec::~Ctverec()
{
    std::cout << "Ctverec destructor" << std::endl;
}

double Ctverec::VypocetObsahu()
{
    return this->strana * this->strana;
}

double Ctverec::VypocetObvodu()
{
    return this->strana * 4;
}