#ifndef STATE_H
#define STATE_H

#include "ATM.h"
#include "Account.h"

using namespace std;

class State {
protected:
    Account* account;
    ATM* atm;
    bool primary;
public:
    State(Account* account, ATM* atm, bool primary) : account(account), atm(atm), primary(primary) {}
    virtual ~State() {}
    virtual void stateAction() = 0; // void or bool func
};


#endif // STATE_H
