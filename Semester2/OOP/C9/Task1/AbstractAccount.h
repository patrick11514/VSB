#ifndef __AbstractAccount_H_
#define __AbstractAccount_H_ 1

class AbstractAccount
{
public:
    AbstractAccount();
    virtual ~AbstractAccount();

    virtual bool CanWithdraw(double a) = 0;
};

#endif
