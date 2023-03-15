#ifndef _FAKTURA_H
#define _FAKTURA_H

#include <string>
#include "Osoba.h"
#include "PolozkaFaktura.h"
using namespace std;

class Faktura
{
private:
    int cislo;
    Osoba *osoba;
    PolozkaFaktura **polozky;
    int count;
    int size;

public:
    Faktura(int cislo);
    Faktura(int cislo, string name, string address);
    Faktura(int cislo, int count);
    Faktura(int cislo, string name, string address, int count);
    ~Faktura();
    void SetCislo(int cislo);
    int GetCislo();
    void SetOsoba(string name, string address);
    void SetPolozkyCount(int count);
    int GetPolozkyCount();
    int GetPolozkySize();
    PolozkaFaktura *addPolozka(string name, double price);
    PolozkaFaktura *GetPolozka(int k);
    PolozkaFaktura *GetPolozka(string name);
    string GetOsobaName();
    string GetOsobaAddress();
    double GetTotalPrice();
};

#endif