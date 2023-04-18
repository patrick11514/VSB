#include "Ctverec.h"

Ctverec::Ctverec(double s) : GeometricObject()
{
    this->strana = s;
}

double Ctverec::VypocetObsahu()
{
    return this->strana * this->strana;
}

double Ctverec::VypocetObvodu()
{
    return this->strana * 4;
}