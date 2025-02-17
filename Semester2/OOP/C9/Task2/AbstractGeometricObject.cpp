#include "AbstractGeometricObject.h"

#include <iostream>

AbstractGeometricObject::AbstractGeometricObject()
{
    std::cout << "AbstractGeometricObject constructor" << std::endl;
}

AbstractGeometricObject::~AbstractGeometricObject()
{
    std::cout << "AbstractGeometricObject destructor" << std::endl;
}
