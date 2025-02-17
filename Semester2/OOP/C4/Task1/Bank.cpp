#include "Bank.h"
#include "Client.h"
#include "Account.h"

Bank::Bank(int c, int a)
{
    this->clientsCount = 0;
    this->clients = new Client *[c];

    this->accountsCount = 0;
    this->accounts = new Account *[a];
}
Bank::~Bank()
{
    for (int i = 0; i < this->clientsCount; i++)
    {
        delete this->clients[i];
    }
    delete[] this->clients;
}

Client *Bank::GetClient(int c)
{
    if (c > this->clientsCount || c < 0)
        return nullptr;
    return this->clients[c];
}
Account *Bank::GetAccount(int n)
{
    if (n > this->accountsCount || n < 0)
        return nullptr;
    return this->accounts[n];
}

Client *Bank::CreateClient(int c, string n)
{
    Client *obj = new Client(c, n);
    this->clients[this->clientsCount] = obj;
    this->clientsCount++;

    return obj;
}
Account *Bank::CreateAccount(int n, Client *c)
{
    Account *obj = new Account(n, c);
    this->accounts[this->accountsCount] = obj;
    this->accountsCount++;

    return obj;
}
Account *Bank::CreateAccount(int n, Client *c, double ir)
{
    Account *obj = new Account(n, c, ir);
    this->accounts[this->accountsCount] = obj;
    this->accountsCount++;

    return obj;
}
Account *Bank::CreateAccount(int n, Client *c, Client *p)
{
    Account *obj = new Account(n, c, p);
    this->accounts[this->accountsCount] = obj;
    this->accountsCount++;

    return obj;
}
Account *Bank::CreateAccount(int n, Client *c, Client *p, double ir)
{
    Account *obj = new Account(n, c, p, ir);
    this->accounts[this->accountsCount] = obj;
    this->accountsCount++;

    return obj;
}

void Bank::AddInterest()
{
    for (int i = 0; i < this->accountsCount; i++)
    {
        this->accounts[i]->AddInterest();
    }
}
