#ifndef STATETRANSFER_H
#define STATETRANSFER_H

#include "STATE.h"

class state_transfer : public State {
private:
public:
	state_transfer(Account* account, ATM* atm, bool primary) : State(account, atm, primary) {};
	~state_transfer() {};
	void stateAction() override;
};

#endif STATETRANSFER_H