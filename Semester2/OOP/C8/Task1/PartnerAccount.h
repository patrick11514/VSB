#ifndef _PARTNER_ACCOUNT_H
#define _PARTNER_ACCOUNT_H

#include "Client.h"
#include "Account.h"
using namespace std;

class PartnerAccount : public Account
{
private:
    Client *partner;

public:
    PartnerAccount(int n, Client *c, Client *p);
    PartnerAccount(int n, Client *c, Client *p, double ir);

    Client *GetPartner();
};

#endif
