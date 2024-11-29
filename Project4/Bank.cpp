#include "BANK.h"

Bank::~Bank() {
    for (auto& pair : accounts) {
        delete pair.second;
    }
}
Account* Bank::createAccount(string user_name, int initial_funds, string password, const string& card_number, const string& account_number) {
    Account* newAccount = new Account(this, user_name, initial_funds, password, card_number, account_number);
    bank_accounts[newAccount->getAccountId()] = newAccount; //account id로 account 저장
    //accounts[newAccount->getAccountId()] = newAccount; 
    return newAccount;
}

Account* Bank::getAccount(int account_id) {
    auto it = accounts.find(account_id); //account_id가 없을 경우 accounts.end()가 반환됨
    return (it != accounts.end()) ? it->second : nullptr; //find 성공 시 second=Account* 반환
}
Bank* Bank::getOrCreateBank(const string& bank_name, vector<Bank>& banks) {
    auto it = find_if(banks.begin(), banks.end(), [&bank_name](const Bank& bank) {
        return bank.getName() == bank_name;
        });

    if (it == banks.end()) {
        // 은행이 존재하지 않으면 새로 생성
        banks.emplace_back(bank_name);
        return &banks.back(); // 새로 생성한 은행을 가리킴
    }
    else {
        // 기존의 은행을 반환
        return &(*it);
    }
}