#include "Obdelnik.h"
#include "GeometricObject.h"

Obdelnik::Obdelnik(double a, double b)
{
    this->stranaA = a;
    this->stranaB = b;
}

void Obdelnik::VypocetObvodu()
{
    this->obvod = this->stranaA * 2 + this->stranaB * 2;
}

void Obdelnik::VypocetObsahu()
{
    this->obsah = this->stranaA * this->stranaB;
}
