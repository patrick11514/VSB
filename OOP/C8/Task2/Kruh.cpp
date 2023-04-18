#include "Kruh.h"
#include "GeometricObject.h"
#include <math.h>

Kruh::Kruh(double r) : GeometricObject()
{
    this->polomer = r;
}

void Kruh::VypocetObsahu()
{
    this->obsah = this->polomer * this->polomer * M_PI;
}

void Kruh::VypocetObvodu()
{
    this->obvod = this->polomer * 2 * M_PI;
}
