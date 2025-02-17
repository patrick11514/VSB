#include <string>
#include <iostream>
#include "PolozkaFaktura.h"

using namespace std;

PolozkaFaktura::PolozkaFaktura()
{
    this->name = "";
    this->price = 0;
}

PolozkaFaktura::PolozkaFaktura(string name, double price)
{
    this->name = name;
    this->price = price;
}

void PolozkaFaktura::SetName(string name)
{
    this->name = name;
}

void PolozkaFaktura::SetPrice(double price)
{
    this->price = price;
}

string PolozkaFaktura::GetName()
{
    return this->name;
}

double PolozkaFaktura::GetPrice()
{
    return this->price;
}