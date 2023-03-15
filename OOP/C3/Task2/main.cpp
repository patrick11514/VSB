#include <iostream>
#include <string>
#include "Faktura.h"
#include "PolozkaFaktura.h"

int main()
{
    Faktura *faktura = new Faktura(59189174, "Patrik Mintěl", "Dolní Domaslavice 239");
    faktura->SetPolozkyCount(5);

    faktura->addPolozka("Rohlík", 1.67);
    faktura->addPolozka("Chleba", 12.56);
    faktura->addPolozka("Počítač", 9458.86);
    faktura->addPolozka("Iphone 13", 22345);
    faktura->addPolozka("Myš", 666.66);

    cout << "Faktura č." << faktura->GetCislo() << endl
         << "Jméno: " << faktura->GetOsobaName() << endl
         << "Adresa: " << faktura->GetOsobaAddress() << endl
         << "Položky: " << endl;
    for (int i = 0; i < faktura->GetPolozkyCount(); i++)
    {
        PolozkaFaktura *obj = faktura->GetPolozka(i);
        cout << "Produkt č." << i + 1 << " | " << obj->GetName() << " | " << obj->GetPrice() << "Kč" << endl;
    }

    cout << "Celková cena: " << faktura->GetTotalPrice() << endl;

    delete faktura;
    return 0;
}