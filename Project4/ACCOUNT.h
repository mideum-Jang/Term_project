#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "CARD.h"

using namespace std;

class Account {
private:
    static int next_id; // 1, 2, 3... generator
    int account_id; // 1, 2, 3... Unique account ID 
    string account_number; //12 digits
    Bank* account_bank; // Pointer to the associated bank
    string account_user_name; // Account holder's name
    string account_password; // Account password
    int account_funds; // Account balance
    //삭제 string connected_card_number; 
    Card* associated_card;
    vector<string> transaction_history; // Transaction history ??

public:
    //삭제 Account(Bank* bank, string user_name, int account_number, int initial_funds, string password, string card_number);
    Account(Bank* bank, string user_name, int initial_funds, string password, const string& card_number, const string& account_number);
    ~Account();

    // Methods
    string getBankName() const; // Get the bank name
    int getAccountId() const { return account_id; }// Get the account ID
    string getAccountNumber() const { return account_number; }
    //삭제 string getConnectedCardNumber() const { return connected_card_number; }
    void addFund(int fund); // Add funds to the account
    bool subFund(int fund); // Subtract funds from the account
    string getUserName() const { return account_user_name; }
    string getPassword() const { return account_password; } // Get the account password
    int getFund() const { return account_funds; } // Get the account balance
    Card* getAssociatedCard() const { return associated_card; } //카드 접근자
    const vector<string>& getTransactionHistory() const; // Get transaction history //??


private:
    void recordTransaction(const string& transaction); // Record a transaction
};

extern vector<Account> accounts;

#endif ACCOUNT_H