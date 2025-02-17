#include <iostream>
#include <string>
#include "GeometricObject.h"
#include "Ctverec.h"
#include "Kruh.h"
#include "Obdelnik.h"

int main()
{
    GeometricObject *obj = new GeometricObject();
    obj->VypocetObsahu();
    obj->VypocetObvodu();
    Ctverec *ctverec = new Ctverec(3);
    ctverec->VypocetObsahu();
    ctverec->VypocetObvodu();
    Kruh *kruh = new Kruh(10);
    kruh->VypocetObsahu();
    kruh->VypocetObvodu();
    Obdelnik *obdelnik = new Obdelnik(5, 10);
    obdelnik->VypocetObsahu();
    obdelnik->VypocetObvodu();

    // print
    std::cout << "GeometricObject: " << std::endl;
    std::cout << "Obvod: " << obj->GetObvod() << std::endl;
    std::cout << "Obsah: " << obj->GetObsah() << std::endl;

    std::cout << "Ctverec: " << std::endl;
    std::cout << "Obvod: " << ctverec->GetObvod() << std::endl;
    std::cout << "Obsah: " << ctverec->GetObsah() << std::endl;

    std::cout << "Kruh: " << std::endl;
    std::cout << "Obvod: " << kruh->GetObvod() << std::endl;
    std::cout << "Obsah: " << kruh->GetObsah() << std::endl;

    GeometricObject *test = ctverec;
    std::cout << "Ctverec jako GeometicObject: " << std::endl;
    std::cout << "Obvod: " << test->GetObvod() << std::endl;
    std::cout << "Obsah: " << test->GetObsah() << std::endl;

    return 0;
}