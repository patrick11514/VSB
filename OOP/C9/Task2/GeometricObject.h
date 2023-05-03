#ifndef __GeometricObject_H_
#define __GeometricObject_H_ 1

#include "AbstractGeometricObject.h"

class GeometricObject : public AbstractGeometricObject
{
public:
    GeometricObject();
    virtual ~GeometricObject();

    double VypocetObsahu();
    double VypocetObvodu();
};

#endif
