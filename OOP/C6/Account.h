#ifndef _ACCOUNT_H
#define _ACCOUNT_H

#include "Client.h"
using namespace std;

class Account
{
private:
    int number;
    double balance;
    double interestRate;

    Client *owner;
    Client *partner;

    static int count;
    static double defaultInterestRate;

    bool defaultInterestRateSet = false;

public:
    Account(int n, Client *c);
    Account(int n, Client *c, double ir);
    Account(int n, Client *c, Client *p);
    Account(int n, Client *c, Client *p, double ir);

    int GetNumber();
    double GetBalance();
    Client *GetOwner();
    Client *GetParner();
    bool CanWithdraw(double a);
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
};

#endif
