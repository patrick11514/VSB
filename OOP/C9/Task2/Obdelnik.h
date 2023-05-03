#ifndef __Obdelnik_H_
#define __Obdelnik_H_ 1

#include "GeometricObject.h"

class Obdelnik : public GeometricObject
{
private:
    double stranaA;
    double stranaB;

public:
    Obdelnik(double a, double b);
    ~Obdelnik();

    double VypocetObvodu();
    double VypocetObsahu();
};

#endif
