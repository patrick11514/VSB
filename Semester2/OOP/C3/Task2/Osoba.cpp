#include <string>
#include <iostream>
#include "Osoba.h"

using namespace std;

Osoba::Osoba()
{
    this->name = "";
    this->address = "";
}

Osoba::Osoba(string name, string address)
{
    this->name = name;
    this->address = address;
}

void Osoba::SetName(string name)
{
    this->name = name;
}
void Osoba::SetAddress(string address)
{
    this->address = address;
}
string Osoba::GetName()
{
    return this->name;
}
string Osoba::GetAddress()
{
    return this->address;
}