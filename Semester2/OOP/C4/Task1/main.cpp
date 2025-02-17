#include <iostream>
#include <string>
#include <random>
#include "Account.h"
#include "Bank.h"
#include "Client.h"

int main()
{
    const int clients = 12;
    const int accounts = 10;

    Bank *bank = new Bank(clients, accounts);

    for (int i = 0; i < accounts; i++)
    {
        int number = random();

        string name, surname;
        cin >> name >> surname;
        name += " " + surname;

        Client *client = bank->CreateClient(i, name);
        Client *partner = nullptr;

        double ir = i * 0.13456;

        if (i == 6 || i == 0)
        {
            cin >> name >> surname;
            name += " " + surname;
            partner = bank->CreateClient(i + 45, name);
        }

        Account *account;
        if (partner == nullptr)
            account = bank->CreateAccount(number, client, ir);
        else
            account = bank->CreateAccount(number, client, partner, ir);

        int balance = 123456;
        switch (i % 3)
        {
        case 0:
            balance *= 1.345;
            break;
        case 1:
            balance *= 2.578;
            break;
        case 2:
            balance *= 0.165;
            break;

        default:
            balance *= 1.112;
            break;
        }

        account->Deposit(balance);

        if (i % 2 == 0)
        {
            account->AddInterest();
        }
    }

    cout << "Výpis lidí v bance: " << endl
         << "        Jméno      |   Kód  " << endl;
    for (int i = 0; i < clients; i++)
    {
        Client *client = bank->GetClient(i);
        cout << client->GetName() << "\t\t" << client->GetCode() << endl;
    }

    cout << endl
         << "Seznam účtů:" << endl;
    for (int i = 0; i < accounts; i++)
    {
        Account *account = bank->GetAccount(i);

        Client *owner = account->GetOwner();
        Client *partner = account->GetParner();

        cout << account->GetNumber()
             << ":\t("
             << owner->GetCode()
             << ") "
             << owner->GetName()
             << "\t|\t"
             << ((partner == nullptr) ? "Není Partner\t" : "(" + to_string(partner->GetCode()) + ") " + partner->GetName())
             << "\t|\t"
             << account->GetBalance()
             << "Kč"
             << "\t Úrok: "
             << account->GetInterest()
             << endl;
    }

    cout << endl
         << "Úročení všech" << endl;
    bank->AddInterest();

    for (int i = 0; i < accounts; i++)
    {
        Account *account = bank->GetAccount(i);

        Client *owner = account->GetOwner();
        Client *partner = account->GetParner();

        cout << account->GetNumber()
             << ":\t("
             << owner->GetCode()
             << ") "
             << owner->GetName()
             << "\t|\t"
             << ((partner == nullptr) ? "Není Partner\t" : "(" + to_string(partner->GetCode()) + ") " + partner->GetName())
             << "\t|\t"
             << account->GetBalance()
             << "Kč"
             << "\t Úrok: "
             << account->GetInterest()
             << endl;
    }

    delete bank;

    return 0;
}