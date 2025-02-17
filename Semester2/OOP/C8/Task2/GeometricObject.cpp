#include "GeometricObject.h"

GeometricObject::GeometricObject()
{
    this->obsah = 0;
    this->obvod = 0;
}

void GeometricObject::VypocetObsahu()
{
}

void GeometricObject::VypocetObvodu()
{
}

double GeometricObject::GetObvod()
{
    return this->obvod;
}

double GeometricObject::GetObsah()
{
    return this->obsah;
}
