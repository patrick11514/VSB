#include "GeometricObject.h"

#include <iostream>

GeometricObject::GeometricObject()
{
    std::cout << "GeometricObject constructor" << std::endl;
}

GeometricObject::~GeometricObject()
{
    std::cout << "GeometricObject destructor" << std::endl;
}

double GeometricObject::VypocetObsahu()
{
    return -1;
}

double GeometricObject::VypocetObvodu()
{
    return -1;
}
