#include <iostream>
#include <string>
#include "GeometricObject.h"
#include "Ctverec.h"
#include "Kruh.h"
#include "Obdelnik.h"

int main()
{
    GeometricObject *obj = new GeometricObject();
    Ctverec *ctverec = new Ctverec(3);
    Kruh *kruh = new Kruh(10);
    Obdelnik *obdelnik = new Obdelnik(5, 10);

    // print
    std::cout << "GeometricObject: " << std::endl;
    std::cout << "Obvod: " << obj->VypocetObvodu() << std::endl;
    std::cout << "Obsah: " << obj->VypocetObsahu() << std::endl;

    std::cout << "Ctverec: " << std::endl;
    std::cout << "Obvod: " << ctverec->VypocetObvodu() << std::endl;
    std::cout << "Obsah: " << ctverec->VypocetObsahu() << std::endl;

    std::cout << "Kruh: " << std::endl;
    std::cout << "Obvod: " << kruh->VypocetObvodu() << std::endl;
    std::cout << "Obsah: " << kruh->VypocetObsahu() << std::endl;

    GeometricObject *test = ctverec;
    std::cout << "Ctverec jako GeometicObject: " << std::endl;
    std::cout << "Obvod: " << test->VypocetObvodu() << std::endl;
    std::cout << "Obsah: " << test->VypocetObsahu() << std::endl;

    return 0;
}