#ifndef __Kruh_H_
#define __Kruh_H_ 1

#include "GeometricObject.h"

class Kruh : public GeometricObject
{
private:
    double polomer;

public:
    Kruh(double p);

    void VypocetObvodu();
    void VypocetObsahu();
};

#endif
