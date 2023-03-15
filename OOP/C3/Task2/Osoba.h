#ifndef _OSOBA_H
#define _OSOBA_H

#include <string>
using namespace std;

class Osoba
{
private:
    string name;
    string address;

public:
    Osoba();
    Osoba(string name, string address);
    void SetName(string name);
    void SetAddress(string address);
    string GetName();
    string GetAddress();
};

#endif