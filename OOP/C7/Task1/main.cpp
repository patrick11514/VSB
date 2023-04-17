#include <iostream>
#include <string>
#include <random>
#include "Account.h"
#include "Bank.h"
#include "Client.h"
#include "PartnerAccount.h"

void printData(int accounts, Bank *bank)
{
    for (int i = 0; i < accounts; i++)
    {
        PartnerAccount *account = (PartnerAccount *)bank->GetAccount(i);

        Client *owner = account->GetOwner();
        Client *partner = account->GetPartner();

        cout << account->GetNumber()
             << ":\t"
             << owner->toString()
             << "\t|\t"
             << ((partner == nullptr) ? "Není Partner\t" : partner->toString())
             << "\t|\t"
             << account->GetBalance()
             << "Kč"
             << "\t Úrok: "
             << account->GetInterest()
             << endl;
    }
}

int main()
{
    const int clients = 12;
    const int accounts = 10;

    Bank *bank = new Bank(clients, accounts);

    Account::setDefaultInteresetRate(0.04);

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
            account = bank->CreateAccount(number, client, partner);

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
         << "[KÓD] Jméno" << endl;
    for (int i = 0; i < clients; i++)
    {
        Client *client = bank->GetClient(i);
        cout << client->toString() << endl;
    }

    cout << endl
         << "Seznam účtů:" << endl;
    printData(accounts, bank);

    cout << endl
         << "Úročení všech" << endl;
    bank->AddInterest();
    printData(accounts, bank);

    cout << endl
         << "Úročení těch se základním úrokem" << endl;
    bank->AddInterestToDefault();
    printData(accounts, bank);

    cout << "Account count: " << Account::getCount() << endl;
    cout << "Client total count: " << Client::getCount() << endl;

    Account *removingAccount = bank->GetAccount(4);

    cout << endl
         << "Mažu účet " << removingAccount->GetNumber() << endl;
    if (bank->removeAccount(removingAccount->GetNumber()))
    {
        cout << "Úspěšně smazáno" << endl;
    }
    else
    {
        cout << "Chyba mazání" << endl;
    }

    printData(accounts - 1, bank);

    delete bank;

    return 0;
}