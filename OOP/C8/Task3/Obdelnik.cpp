#include "Obdelnik.h"
#include "GeometricObject.h"

Obdelnik::Obdelnik(double a, double b)
{
    this->stranaA = a;
    this->stranaB = b;
}

double Obdelnik::VypocetObvodu()
{
    return this->stranaA * 2 + this->stranaB * 2;
}

double Obdelnik::VypocetObsahu()
{
    return this->stranaA * this->stranaB;
}
