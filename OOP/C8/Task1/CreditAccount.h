#ifndef __CreditAccount_H_
#define __CreditAccount_H_ 1

#include "Account.h"

class CreditAccount : public Account
{
private:
    double credit;

public:
    CreditAccount(int n, Client *o, double c);
    CreditAccount(int n, Client *o, double ir, double c);

    bool CanWithdraw(double a);
    bool Withdraw(double a);
};

#endif
