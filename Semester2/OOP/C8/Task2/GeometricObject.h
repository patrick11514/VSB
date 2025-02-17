#ifndef __GeometricObject_H_
#define __GeometricObject_H_ 1

class GeometricObject
{
protected:
    double obsah;
    double obvod;

public:
    GeometricObject();

    double GetObsah();
    double GetObvod();
    void VypocetObsahu();
    void VypocetObvodu();
};

#endif
