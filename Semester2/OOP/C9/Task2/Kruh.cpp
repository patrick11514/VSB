#include "Kruh.h"
#include "GeometricObject.h"

#include <math.h>
#include <iostream>

Kruh::Kruh(double r) : GeometricObject()
{
    std::cout << "Kruh constructor" << std::endl;
    this->polomer = r;
}

Kruh::~Kruh()
{
    std::cout << "Kruh destructor" << std::endl;
}

double Kruh::VypocetObsahu()
{
    return this->polomer * this->polomer * M_PI;
}

double Kruh::VypocetObvodu()
{
    return this->polomer * 2 * M_PI;
}
