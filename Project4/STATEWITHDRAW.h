#ifndef STATEWITHDRAW_H
#define STATEWITHDRAW_H

#include "STATE.h"

class state_withdraw : public State {
private:
public:
	state_withdraw(Account* account, ATM* atm, bool primary) : State(account, atm, primary) {};
	~state_withdraw() {};
	void stateAction() override;
};


#endif STATEWITHDRAW_H