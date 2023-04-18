#ifndef __Ctverec_H_
#define __Ctverec_H_ 1

#include "GeometricObject.h"

class Ctverec : public GeometricObject
{
private:
    double strana;

public:
    Ctverec(double s);

    double VypocetObvodu();
    double VypocetObsahu();
};

#endif
