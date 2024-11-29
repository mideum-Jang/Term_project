#ifndef CARD_H
#define CARD_H

#include <iostream>
#include "ACCOUNT.h"
#include "BANK.h"

using namespace std;

class Card {
private:
    string card_number;
    Account* card_account;
    Bank* card_bank;

public:
    Card(Account* account, Bank* bank, string card_num);
    ~Card();
    string getCardNumber() const { return card_number; }
    string getBankname() { return card_bank->getName(); };
    Account* getAccount() { return card_account; };
};

#endif CARD_H