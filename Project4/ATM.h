#ifndef ATM_H
#define ATM_H

#include "Bank.h"
#include <iostream>
#include <unordered_map>
#include "Cash.h"
#include <vector>
#include <sstream>
#include "STATE.h"
#include <algorithm>

using namespace std;

class ATM {
private:
    //Bank& bank; // Reference to the bank
    Bank* primary_bank;
    string serial_number; // Serial number of the ATM
    string type; // Type of ATM
    string language; // Language setting
    //unordered_map<int, int> cash_available; // Cash available in the ATM
    Cash* cash;
    unordered_map<int, int> cash_dispensed;
    int withdrawal_fee;
    int remaining_amount;
    unordered_map<int, int> available_cash;
    vector<int> denominations;
    int amount;
    vector<Bank>& banks;
    State* currentState;
    Account* current_account;
    vector<string> transaction_history; 

public:
    //ATM(Bank* bank, string serial_number, string type, string language, const unordered_map<int, int>& initial_cash);
    ATM(Bank* bank, const string& serial_number, const string& type, const string& language, const unordered_map<int, int>& initial_cash, vector<Bank>& banks_ref);
    ~ATM() { delete currentState; }

    void addCash(int denomination, int count);
    void printAvailableCash() const;
    int getTotalAvailableCash() const;
    //int getAvailableCash();

    Bank* getBank();
    const vector<Bank>& getBanks() const { return banks; }

    int deposit(Account* account, unordered_map<int, int>& cash_deposited); //다른데서도 primary확인해야 함
    string withdraw(int amount);
    bool withdrawAvailable(int amount);
    string cashTransfer(Account* destination, int amount, int fee);
    string accountTransfer(Account* source, Account* destination, int amount);

    string checkBalance(int account_id);
    string getSerialNumber() const { return serial_number; };
    void printTransactionHistory(int account_id);  // Print the transaction history of an account
    void printATMInfo() const;
    bool isValidCard(const string& card_number);
    bool isCorrectPassword(const string& card_number, const string& password);
    void selectATM(vector<ATM>& atms);

    void setCurrentAccount(Account* account) { current_account = account; } // 카드 설정
    void resetCurrentAccount() { current_account = nullptr; }
    Account* getCurrentAccount() const { return current_account; }

    void setState(State* state);

    vector<string> getTransactionHistory();
    void recordTransactionHistory(string rec);
    bool is_primary(Account* account);
    bool getTransactionAvailable(bool primary);

    unordered_map<int, int> makeCashDeposited();        
    unordered_map<int, int> makeFeeDeposited(int fee);
};

extern vector<ATM> atms;

#endif ATM_H