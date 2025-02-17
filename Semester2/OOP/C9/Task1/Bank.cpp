#include "Bank.h"
#include "Client.h"
#include "Account.h"
#include "PartnerAccount.h"
#include "CreditAccount.h"

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
PartnerAccount *Bank::CreateAccount(int n, Client *c, Client *p)
{
    PartnerAccount *obj = new PartnerAccount(n, c, p);
    this->accounts[this->accountsCount] = obj;
    this->accountsCount++;

    return obj;
}
PartnerAccount *Bank::CreateAccount(int n, Client *c, Client *p, double ir)
{
    PartnerAccount *obj = new PartnerAccount(n, c, p, ir);
    this->accounts[this->accountsCount] = obj;
    this->accountsCount++;

    return obj;
}

CreditAccount *Bank::CreateCreditAccount(int n, Client *c, double credit)
{
    CreditAccount *obj = new CreditAccount(n, c, credit);
    this->accounts[this->accountsCount] = obj;
    this->accountsCount++;

    return obj;
}

CreditAccount *Bank::CreateCreditAccount(int n, Client *c, double ir, double credit)
{
    CreditAccount *obj = new CreditAccount(n, c, ir, credit);
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

void Bank::ChangeDefaultInterest(double ir)
{
    Account::setDefaultInteresetRate(ir);
    for (int i = 0; i < this->accountsCount; i++)
    {
        if (this->accounts[i]->HasDefaultInterestRate())
            this->accounts[i]->ChangeInterest(ir);
    }
}

void Bank::AddInterestToDefault()
{
    for (int i = 0; i < this->accountsCount; i++)
    {
        if (this->accounts[i]->HasDefaultInterestRate())
            this->accounts[i]->AddInterest();
    }
}

bool Bank::removeAccount(int number)
{
    for (int i = 0; i < this->accountsCount; i++)
    {
        if (this->accounts[i]->GetNumber() == number)
        {
            delete this->accounts[i];
            // move all
            for (int l = i; l < this->accountsCount - 1; l++)
            {
                this->accounts[l] = this->accounts[l + 1];
                this->accounts[l + 1] = nullptr;
            }

            this->accountsCount--;

            return true;
        }
    }

    return false;
}

bool Bank::removeClient(int code)
{
    for (int i = 0; i < this->clientsCount; i++)
    {
        if (this->clients[i]->GetCode() == code)
        {
            // Find account
            for (int l = 0; l < this->accountsCount; l++)
            {
                if (this->accounts[l]->GetOwner()->GetCode() == code)
                {
                    this->removeAccount(this->accounts[l]->GetNumber());
                    break;
                }
            }

            delete this->clients[i];

            // move all
            for (int l = i; l < this->clientsCount - 1; i++)
            {
                this->clients[l] = this->clients[l + 1];
                this->clients[l + 1] = nullptr;
            }

            this->clientsCount--;

            return true;
        }
    }

    return false;
}