#include "Account.h"

Account::Account(int n, Client *c)
{
    this->number = n;
    this->owner = c;
}
Account::Account(int n, Client *c, double ir)
{
    this->number = n;
    this->owner = c;
    this->interestRate = ir;
}
Account::Account(int n, Client *c, Client *p)
{
    this->number = n;
    this->owner = c;
    this->partner = p;
}
Account::Account(int n, Client *c, Client *p, double ir)
{
    this->number = n;
    this->owner = c;
    this->partner = p;
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
Client *Account::GetParner()
{
    return this->partner;
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