#ifndef __AbstractGeometricObject_H_
#define __AbstractGeometricObject_H_ 1

class AbstractGeometricObject
{
public:
    AbstractGeometricObject();
    virtual ~AbstractGeometricObject();

    virtual double VypocetObsahu() = 0;
    virtual double VypocetObvodu() = 0;
};

#endif
