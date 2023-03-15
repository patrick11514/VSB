#include <string>
#include <iostream>
#include "Faktura.h"
#include "Osoba.h"
#include "PolozkaFaktura.h"

using namespace std;

Faktura::Faktura(int cislo)
{
    this->cislo = cislo;
    this->osoba = nullptr;
    this->polozky = nullptr;
    this->count = 0;
    this->size = 0;
}

Faktura::Faktura(int cislo, string name, string address)
{
    this->osoba = new Osoba(name, address);
    this->polozky = nullptr;
    this->cislo = cislo;
    this->count = 0;
    this->size = 0;
}

Faktura::Faktura(int cislo, int count)
{
    this->osoba = nullptr;
    this->polozky = new PolozkaFaktura *[count];
    this->cislo = cislo;
    this->count = 0;
    this->size = count;
}

Faktura::Faktura(int cislo, string name, string address, int count)
{
    this->osoba = new Osoba(name, address);
    this->polozky = new PolozkaFaktura *[count];
    this->cislo = cislo;
    this->count = 0;
    this->size = count;
}

Faktura::~Faktura()
{
    delete this->osoba;
    for (int i = 0; i < this->count; i++)
    {
        delete this->polozky[i];
    }
    delete[] this->polozky;
    this->size = 0;
    this->count = 0;
}

void Faktura::SetCislo(int cislo)
{
    this->cislo = cislo;
}

int Faktura::GetCislo()
{
    return this->cislo;
}

void Faktura::SetOsoba(string name, string address)
{
    this->osoba = new Osoba(name, address);
}
void Faktura::SetPolozkyCount(int count)
{
    this->polozky = new PolozkaFaktura *[count];
    this->size = count;
}
int Faktura::GetPolozkyCount()
{
    return this->count;
}
int Faktura::GetPolozkySize()
{
    return this->size;
}
PolozkaFaktura *Faktura::addPolozka(string name, double price)
{
    if (this->size == 0 || this->count == this->size)
        return nullptr;
    PolozkaFaktura *obj = new PolozkaFaktura(name, price);
    this->polozky[this->count] = obj;
    this->count++;
    return obj;
}
PolozkaFaktura *Faktura::GetPolozka(int k)
{
    if (this->count - 1 >= k && k >= 0)
    {
        return this->polozky[k];
    }
    return nullptr;
}
PolozkaFaktura *Faktura::GetPolozka(string name)
{
    for (int i = 0; i < this->count; i++)
    {
        if (this->polozky[i]->GetName() == name)
            return this->polozky[i];
    }
    return nullptr;
}
string Faktura::GetOsobaName()
{
    if (this->osoba == nullptr)
        return nullptr;
    return this->osoba->GetName();
}
string Faktura::GetOsobaAddress()
{
    if (this->osoba == nullptr)
        return nullptr;
    return this->osoba->GetAddress();
}

double Faktura::GetTotalPrice()
{
    double price = 0;
    for (int i = 0; i < this->count; i++)
    {
        price += this->polozky[i]->GetPrice();
    }
    return price;
};