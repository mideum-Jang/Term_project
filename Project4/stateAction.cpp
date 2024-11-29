#include "STATE.h"
#include "STATEATMRECEIPT.h"
#include "STATEACCOUNTRECEIPT.h"
#include "STATESNAPSHOT.h"
#include "STATEDEPOSIT.h"
#include "STATEWITHDRAW.h"
#include "STATETRANSFER.h"


void state_ATM_receipt::stateAction() {
	// verify admin card

	vector<string> rec = atm->getTransactionHistory();
	for (string vec : rec) {
		cout << vec << endl;
	}
}

void state_account_receipt::stateAction() {
	vector<string> rec = this->account->getTransactionHistory();
	for (string vec : rec) {
		cout << vec << endl;
	}
}

void state_snapshot::stateAction() {
	ostringstream oss;
	for (ATM& vec : atms) {
		oss << "ATM [SN: " << vec.getSerialNumber() << "] "
			<< "remaining cash: {" << vec.getTotalAvailableCash() << "}\n";
	}
	for (Account& vec : accounts) {
		oss << "Account [Bank: " << vec.getBankName() << ", "
			<< "No: " << vec.getAccountNumber() << ", "
			<< "Owner: " << vec.getUserName() << "] "
			<< "Balance: " << vec.getFund() << "\n";
	}
	cout << oss.str();
}

void state_deposit::stateAction() {
	ostringstream oss;

	int deposit_fee = primary ? 1000 : 2000;

	int choice;
	cout << "Please select your deposit method." << endl;
	cout << "1. Card deposit\n 2. Check deposit"; //입력이 숫자가 아니면? exception handling
	if (choice == 1) {
		unordered_map<int, int> cash_deposited = atm->makeCashDeposited(); // 현금 입금 내역
		unordered_map<int, int> fee_deposited = atm->makeFeeDeposited(deposit_fee);

		if (fee_deposited[1000] * 1000 == deposit_fee) {
			int fund_amount = atm->deposit(account, cash_deposited);
			account->addFund(fund_amount);
			atm->deposit(account, fee_deposited);
			oss << "Deposit successful. New balance: " << account->getFund();
		}
		else
			oss << "The fee amount inserted is incorrect.";
	}
	else if (choice == 2) {
		int check;
		cout << "Insert the check: ";
		cin >> check;
		if (check < 100000) 
			oss << "Checks must exceed 100,000 KRW.";
		else {
			unordered_map<int, int> fee_deposited;
			cout << "Enter the deposit fee: " << deposit_fee << endl;
			cout << "Enter number of KRW 1,000 bills: ";
			cin >> fee_deposited[1000];

			if (fee_deposited[1000] * 1000 == deposit_fee) {
				account->addFund(check);
				atm->deposit(account, fee_deposited);
				oss << "Deposit successful. New balance: " << account->getFund();
			}
			else
				oss << "The fee amount inserted is incorrect.";
		}
	}
	else {
		oss << "Invalid selection. Returning to the main interface.";
	}
	cout << oss.str() << endl;
}

void state_withdraw::stateAction() {
	ostringstream oss;

	int withdrawal_fee = primary ? 1000 : 2000;

	int amount;
	cout << "Please enter the amount to withdraw." << endl;
	cout << "Withdrawal Amount: ";
	cin >> amount;	

	if (amount > 500000) {
		cout << "The maximum withdrawal amount is 500,000 KRW." << endl;
		return;
	}
	else if (amount % 1000 != 0) {
		cout << "Invalide amount." << endl;
		return;
	}
	else if (amount <= withdrawal_fee) {
		cout << "그걸 왜 함?" << endl;
		return;
	}

	int remaining_amount = amount - withdrawal_fee;

	if (remaining_amount > atm->getTotalAvailableCash()) {
		cout << "Insufficient cash available to dispense the requested amount including fees." << endl;
		return;
	}
	if (remaining_amount > account->getFund()) {
		cout << "Insufficient account balance." << endl;
		return;
	}

	bool avail = atm->withdrawAvailable(remaining_amount);
	if (avail) {
		string result = atm->withdraw(remaining_amount);
		account->subFund(remaining_amount);
		oss << result;
	}
	else 
		oss << "There is not enough cash in the ATM.";
	
	cout << oss.str();
}

void state_transfer::stateAction() {
	ostringstream oss;

	int destination_account_number;
	string destination_bank_name;
	cout << "Enter destination bank name: ";
	cin >> destination_bank_name;
	cout << "Enter destination account number: ";
	cin >> destination_account_number;

	Bank* destination_bank;
	for (Bank vec : banks) {
		if (vec.getName() == destination_bank_name)
			destination_bank = &vec;
	}
	// 찾기 못한 경우 exception handling
	if (!destination_bank) {
		cout << "Destination bank not found.\n";
		return;
	}
	
	Account* destination = destination_bank->getAccount(destination_account_number);


	int transfer_type, amount;
	cout << "Choose transfer type:\n1. Cash Transfer\n2. Account Transfer\n Select Type: ";
	cin >> transfer_type;

	int transfer_fee;
	if (transfer_type == 1) {
		transfer_fee = 1000;
		if (amount <= transfer_fee) {
			cout << "그걸 왜함" << endl;
		}

		oss << atm->cashTransfer(destination, amount, transfer_fee);
	}
	else if (transfer_type == 2) {
		cout << "Please enter the amount to transfer." << endl;
		cout << "Transfer Amount: ";
		cin >> amount;

		if (primary and (account->getBankName() == destination->getBankName()))
			transfer_fee = 2000;
		else if (!primary and (account->getBankName() != destination->getBankName()))
			transfer_fee = 4000;
		else
			transfer_fee = 3000;
		if (amount <= transfer_fee) {
			cout << "그걸 왜함" << endl;
		}

		oss << atm->accountTransfer(account, destination, amount-transfer_fee);
	}
	else 
		cout << "Invalid transfer type selected.\n";

	cout << oss.str() << endl;
}