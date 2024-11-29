#include "ACCOUNT.h"

Account::Account(Bank* bank, string user_name, int initial_funds, string password, const string& card_number, const string& account_number)
//���� : account_bank(bank), account_user_name(user_name), account_password(password), connected_card_number(card_number), account_funds(initial_funds), account_id(account_number) {
    : account_bank(bank), account_user_name(user_name), account_password(password), account_funds(initial_funds), account_number(account_number) {
    account_id = next_id++;
    associated_card = new Card(this, bank, card_number);
    recordTransaction("Initial deposit: " + to_string(initial_funds)); //??
}

Account::~Account() {}

string Account::getBankName() const {
    return account_bank->getName();
}

void Account::addFund(int fund) {
    account_funds += fund;
    recordTransaction("Deposited: " + to_string(fund)); //??
}

bool Account::subFund(int fund) {
    if (fund > 0 && fund <= account_funds) {
        account_funds -= fund;
        recordTransaction("Withdrew: " + to_string(fund)); //??
        return true;
    }
    return false;
}

// ?? Record a transaction
void Account::recordTransaction(const string& transaction) {
    transaction_history.push_back(transaction);
}

// ?? Get the transaction history //session���� �� ���?
const vector<string>& Account::getTransactionHistory() const {
    return transaction_history;
}

// Implementation of Bank methods
Account* Bank::createAccount(string user_name, int initial_funds, string password, const string& card_number, const string& account_number) {
    Account* newAccount = new Account(this, user_name, initial_funds, password, card_number, account_number);
    bank_accounts[newAccount->getAccountId()] = newAccount; //account id�� account ����
    //accounts[newAccount->getAccountId()] = newAccount; 
    return newAccount;
}

Account* Bank::getAccount(int account_id) {
    auto it = accounts.find(account_id); //account_id�� ���� ��� accounts.end()�� ��ȯ��
    return (it != accounts.end()) ? it->second : nullptr; //find ���� �� second=Account* ��ȯ
}

vector<Account> accounts;