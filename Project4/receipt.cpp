#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
using namespace std;

// Account.getReceipt(), Account receipt 변수
// ATM.getReceipt(), ATM receipt 변수
// verify admin card -> admin card 입력 방식?


class State {
protected:
	ATM* state_ATM;
	Account* state_account;
public:
	State(ATM* atm, Account* account) { state_ATM = atm;  state_account = account; };
	virtual ~State() {};
	virtual void stateAction() = 0; // void or bool func
};

class state_account_receipt : public State{
private:
public:
	state_account_receipt(Account* account): State(atm, account) {};
	~state_account_receipt() {};
	void stateAction() override;
};
void state_account_receipt::stateAction() {
	cout << this->state_account->getReceipt() << endl;
}

class state_ATM_receipt : public State {
private:
public:
	state_ATM_receipt(Account* account) : State(atm, account) {};
	~state_ATM_receipt() {};
	void stateAction() override;
};
void state_ATM_receipt::stateAction() {
	// verify admin card

	vector<string> rec = state_ATM->getReceipt();
	for (string vec : rec) {
		cout << vec << endl;
	}
}