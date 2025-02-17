#include "Account.h"
#include "Client.h"

int Account::count = 0;
double Account::defaultInterestRate = 0.0;

Account::Account(int n, Client *c)
{
    Account::count++;
    this->number = n;
    this->owner = c;
    this->interestRate = Account::defaultInterestRate;
    this->defaultInterestRateSet = true;
}
Account::Account(int n, Client *c, double ir)
{
    Account::count++;
    this->number = n;
    this->owner = c;
    this->interestRate = ir;
}

int Account::GetNumber()
{
    return this->number;
}
double Account::GetBalance()
{
    return this->balance;
}
Client *Account::GetOwner()
{
    return this->owner;
}
bool Account::Withdraw(double a)
{
    if (a > this->balance)
        return false;

    this->balance -= a;
    return true;
}
bool Account::CanWithdraw(double a)
{
    if (a > this->balance)
        return false;
    return true;
}

void Account::Deposit(double a)
{
    this->balance += a;
}
void Account::AddInterest()
{
    this->balance += this->balance * this->interestRate;
}

double Account::GetInterest()
{
    return this->interestRate;
}

void Account::ChangeInterest(double ir)
{
    this->interestRate = ir;
}

bool Account::HasDefaultInterestRate()
{
    return this->defaultInterestRateSet;
}