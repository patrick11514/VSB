#ifndef _ACCOUNT_H
#define _ACCOUNT_H

#include "Client.h"
#include "AbstractAccount.h"
using namespace std;

class Account : public AbstractAccount
{
private:
    int number;
    double interestRate;

    Client *owner;

    static int count;
    static double defaultInterestRate;

    bool defaultInterestRateSet = false;

protected:
    double balance;
    int type;

public:
    Account(int n, Client *c);
    Account(int n, Client *c, double ir);
    virtual ~Account();

    int GetNumber();
    double GetBalance();
    Client *GetOwner();
    virtual bool CanWithdraw(double a);
    double GetInterest();

    void Deposit(double a);
    bool Withdraw(double a);
    void AddInterest();
    void ChangeInterest(double ir);
    bool HasDefaultInterestRate();

    static int getCount()
    {
        return count;
    }

    static void setDefaultInteresetRate(double rate)
    {
        defaultInterestRate = rate;
    }

    int getType();
};

#endif
