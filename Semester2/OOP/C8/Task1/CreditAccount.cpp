#include "CreditAccount.h"
#include "Account.h"
#include "enums.h"
#include <iostream>

CreditAccount::CreditAccount(int n, Client *o, double c) : Account(n, o)
{
    this->credit = c;
    this->type = AccountType::Credit;
}
CreditAccount::CreditAccount(int n, Client *o, double ir, double c) : Account(n, o, ir)
{
    this->credit = c;
    this->type = AccountType::Credit;
}

bool CreditAccount::CanWithdraw(double a)
{
    return (this->GetBalance() + this->credit) >= a;
}

bool CreditAccount::Withdraw(double a)
{
    if (a > this->balance)
        return false;

    this->balance -= a;
    return true;
}
