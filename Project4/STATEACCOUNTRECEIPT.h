#ifndef STATEACCOUNTRECEIPT_H
#define STATEACCOUNTRECEIPT_H

#include "STATE.h"

class state_account_receipt : public State {
private:
public:
	state_account_receipt(Account* account, ATM* atm, bool primary) : State(account, atm, primary) {};
	~state_account_receipt() {};
	void stateAction() override;
};


#endif STATEACCOUNTRECEIPT_H