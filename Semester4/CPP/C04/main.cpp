#include <iostream>
#include <string>

int main()
{
    // konstantní pointer na nekonstantní data
    char *const aa = "bla";
    // nekonstantní pointer na konstantní data
    const char *bb = "bla2";
    // můžu
    bb++;
    ++bb;
    // konstantní pointer na konstantní data
    const char *const cc = "bla3";

    return 0;
}
