#include <iostream>
#include <sstream>
#include <iomanip>
using namespace std;



class State {
protected:
	ATM* state_ATM;
	Account* state_account;
public:
	State(ATM* atm, Account* account) { state_ATM = atm;  state_account = account; };
	virtual ~State() {};
	virtual void stateAction() = 0; // void or bool func
};

/*
// child templete
class child : public State{
private:
public:
	child(Account* account): State(atm, account) {};
	~child() {};
	void stateAction() override;
};
*/

// atm(vector), account(vector), ATM.getSN(), ATM.getCash()
// Account.getBankname, getNumber, getUsername, getBalance

class state_snapshot : public State {
private:
public:
	state_snapshot(Account* account) : State(atm, account) {};
	~state_snapshot() {};
	void stateAction() override;
};
void state_snapshot::stateAction() {
	ostringstream oss;
	for (ATM& vec : atms) {
		oss << "ATM [SN: " << vec.getSN() << "] "
			<< "remaining cash: {" << vec.getCash() << "}\n";
	}
	for (Account& vec :accounts ) {
		oss << "Account [Bank: " << vec.getBankname() << ", "
			<< "No: " << vec.getNumber() << ", "
			<< "Owner: " << vec.getUsername() << "] "
			<< "Balance: " << vec.getBalance() << "\n";
	}
	cout << oss.str();
}