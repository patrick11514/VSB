#include "Account.h"
#include "PartnerAccount.h"
#include "Client.h"
#include "enums.h"

PartnerAccount::PartnerAccount(int n, Client *c, Client *p) : Account(n, c)
{
    this->partner = p;
    this->type = AccountType::Partner;
}
PartnerAccount::PartnerAccount(int n, Client *c, Client *p, double ir) : Account(n, c, ir)
{
    this->partner = p;
    this->type = AccountType::Partner;
}
Client *PartnerAccount::GetPartner()
{
    return this->partner;
}