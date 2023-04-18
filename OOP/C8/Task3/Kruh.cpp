#include "Kruh.h"
#include "GeometricObject.h"
#include <math.h>

Kruh::Kruh(double r) : GeometricObject()
{
    this->polomer = r;
}

double Kruh::VypocetObsahu()
{
    return this->polomer * this->polomer * M_PI;
}

double Kruh::VypocetObvodu()
{
    return this->polomer * 2 * M_PI;
}
