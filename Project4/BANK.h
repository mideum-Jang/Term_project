#ifndef BANK_H
#define BANK_H

#include <string>
#include <iostream>
#include <unordered_map>
#include <algorithm>
#include "CASH.h"
#include "ACCOUNT.h"

using namespace std;

class Bank {
private:
    string name; // Bank name
    unordered_map<int, Account*> bank_accounts; // Map of accounts of bank

public:
    Bank(const string& bank_name) : name(bank_name) {}
    string getName() const { return name; }
    Account* createAccount(string user_name, int initial_funds, string password, const string& card_number, const string& accout_number);// Create a new account
    Account* getAccount(int account_id); // Get an account by ID
    const unordered_map<int, Account*>& getAccounts() const { return bank_accounts; }
    ~Bank();// Destructor to clean up accounts of bank
    static Bank* getOrCreateBank(const string& bank_name, vector<Bank>& banks);
};

extern vector<Bank> banks;

#endif BANK_H