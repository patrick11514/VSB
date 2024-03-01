#include "tasks.h"

int main()
{
    UTF8String a{"Ahoj"};
    std::cout << "===== a =====" << std::endl;
    // a.print();
    UTF8String b{" Svete"};
    std::cout << "===== b =====" << std::endl;
    // b.print();

    UTF8String c = a + b;

    std::cout << "===== c =====" << std::endl;
    // c.print();
}
