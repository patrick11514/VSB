#include "Ctverec.h"

Ctverec::Ctverec(double s) : GeometricObject()
{
    this->strana = s;
}

void Ctverec::VypocetObsahu()
{
    this->obsah = this->strana * this->strana;
}

void Ctverec::VypocetObvodu()
{
    this->obvod = this->strana * 4;
}