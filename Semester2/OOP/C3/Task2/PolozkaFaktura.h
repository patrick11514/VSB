#ifndef _POLOZKA_FAKTURA_H
#define _POLOZKA_FAKTURA_H

#include <string>
using namespace std;

class PolozkaFaktura
{
private:
    string name;
    double price;

public:
    PolozkaFaktura();
    PolozkaFaktura(string name, double price);
    void SetName(string name);
    void SetPrice(double price);
    string GetName();
    double GetPrice();
};

#endif