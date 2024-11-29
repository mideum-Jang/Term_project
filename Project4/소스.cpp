#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <iomanip> // for std::setprecision
#include <sstream>
#include <memory> // for std::unique ??
#include <algorithm>

using namespace std;

// 수표 경우의 수 반영 필요
//language, ATM type 반영 필요

// Forward declaration of Bank class
class Bank;
class Account;
class ATM;
class Cash;
class Card;
class State;

// Account class
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

// Initialize static member
int Account::next_id = 1;

// Bank class
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

// Cash class
class Cash {
private:
    unordered_map<int, int> cash_available;
    int cash_value;
    //각 ATM에 종속되는 별개의 cash가 필요  atm1_cash=new Cash() 이런식?
public:
    Cash() : cash_value(0) {}
    //각 ATM에 종속되는 별개의 cash 생성 필요
    //virtual ~Cash() {}
    int getValue() const { return cash_value; };//총 금액 반환 cash_value를 변동이 있을때마다 매번 업데이트해야 함
    //총 가용지폐 현황 반환
    //unordered_map<int, int> getAvailableCash() const {
    unordered_map<int, int> getAvailableCash() {
        return cash_available;
    }

    int getTotalAvailableCash() const { // 총 가용 현금을 계산하는 메서드
        int total = 0;
        for (const auto& pair : cash_available) {
            total += pair.first * pair.second; // 권종 * 수량
        }
        return total;
    }
    void addCash(int denomination, int count);
    void printAvailableCash() const;
    void subCash(int denomination, int count);
};


//different denominations
class KRW_1000 : public Cash {
public:
    KRW_1000() {
        addCash(1000, 0);
    }
};
class KRW_5000 : public Cash {
public:
    KRW_5000() {
        addCash(5000, 0);
    }
};
class KRW_10000 : public Cash {
public:
    KRW_10000() {
        addCash(10000, 0);
    }
};
class KRW_50000 : public Cash {
public:
    KRW_50000() {
        addCash(50000, 0);
    }
};

// ATM class
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
    account* current_account;

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
    string deposit_check(int account_id, bool is_primary);
    string deposit(int account_id, unordered_map<int, int>& cash_deposited, bool is_primary); //다른데서도 primary확인해야 함
    string withdraw(int account_id, int amount, bool is_primary);
    string transfer();
    string checkBalance(int account_id);
    string getSerialNumber() const { return serial_number; };
    void printTransactionHistory(int account_id);  // Print the transaction history of an account
    void printATMInfo() const;
    bool isValidCard(const string& card_number);
    bool isCorrectPassword(const string& card_number, const string& password);
    void startSession();//얘 필요없나?
    void selectATM(vector<ATM>& atms);
    void admin_menu();

    void setCurrentAccount(Account* account) { current_account = account; } // 카드 설정
    void resetCurrentAccount() { current_account = nullptr; }
    Account* getCurrentAccount() const { return current_account; }

    void setState(State* state);
    void performCurrentAction();
};


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

Card::Card(Account* account, Bank* bank, string card_num) : card_account(account), card_bank(bank), card_number(card_num) {}
Card::~Card() {}

class State {
protected:
    Account* state_account;
    ATM* atm;
public:
    State(Account* account, ATM* atm) : state_account(account), atm(atm) {}
    virtual ~State() {}
    virtual void stateAction() = 0; // void or bool func
    virtual void stateAction(ATM& atm) = 0; // 순수 가상 함수
};

//Implementation of Cash methods
void Cash::addCash(int denomination, int count) {
    cash_available[denomination] += count;
    cash_value += denomination * count; // 총 금액 업데이트
}
void Cash::subCash(int denomination, int count) {
    if (cash_available[denomination] >= count) {
        cash_available[denomination] -= count;
    }
}

void Cash::printAvailableCash() const {
    for (const auto& cash : cash_available) {
        cout << "KRW " << cash.first << ": " << cash.second << " bills\n";
    }
}



//Implementation of Account methods
//삭제 Account::Account(Bank* bank, string user_name, int account_number, int initial_funds, string password, string card_number)
Account::Account(Bank* bank, string user_name, int initial_funds, string password, const string& card_number, const string& account_number)
//삭제 : account_bank(bank), account_user_name(user_name), account_password(password), connected_card_number(card_number), account_funds(initial_funds), account_id(account_number) {
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

void Account::subFund(int fund) {
    if (fund > 0 && fund <= account_funds) {
        account_funds -= fund;
        recordTransaction("Withdrew: " + to_string(fund)); //??
    }
}

// ?? Record a transaction
void Account::recordTransaction(const string& transaction) {
    transaction_history.push_back(transaction);
}

// ?? Get the transaction history //session종료 시 출력?
const vector<string>& Account::getTransactionHistory() const {
    return transaction_history;
}




// Implementation of Bank methods
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

Bank::~Bank() {
    for (auto& pair : accounts) {
        delete pair.second;
    }
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
// Implementation of ATM methods
ATM::ATM(Bank* bank, const string& serial_number, const string& type, const string& language, const unordered_map<int, int>& initial_cash, vector<Bank>& banks_ref)
    : primary_bank(bank), serial_number(serial_number), type(type), language(language), banks(banks_ref) {
    for (const auto& pair : initial_cash) {
        cash->addCash(pair.first, pair.second);
    }
}

Bank* ATM::getBank() {
    return primary_bank;
}

void ATM::addCash(int denomination, int count) {
    cash->addCash(denomination, count);// 필요한가?
}

void ATM::printAvailableCash() const {
    cash->printAvailableCash();
}
int ATM::getTotalAvailableCash() const {
    return cash->getTotalAvailableCash();
}

string ATM::deposit_check(int account_id, bool is_primary) {
    Account* account = primary_bank->getAccount(account_id);
    // 유효한 계좌인지 확인
    if (account) {
        int check;
        cout << "Insert the check: ";
        cin >> check;
        // 수표 금액이 100,000원 이상인지 확인
        if (check >= 100000) {
            int inserted_fee = 0;
            int deposit_fee = is_primary ? 1000 : 2000; // Set fee based on bank type
            cout << "Please insert the fee. After inserting, enter the amount of the inserted.\nThe fee: " << deposit_fee;
            cin >> inserted_fee;
            // 수수료 맞게 입력했는지 확인
            if (deposit_fee == inserted_fee) {
                account->addFund(check);
                return "Deposit successful. New balance: " + to_string(account->getFund());
            }
        }
        return "The fee amount inserted is incorrect.";
    }
    return "Error: Account ID " + to_string(account_id) + " not found.";
}

string ATM::deposit(int account_id, unordered_map<int, int>& cash_deposited, bool is_primary) {
    Account* account = primary_bank->getAccount(account_id);
    //cout을 여기서 하지 말고, 문자열을 반환하기, ATM 내부 함수는 action만 하고 출력은  외부에서

    if (account) {
        int inserted_fee = 0;
        int total_deposit = 0;

        int deposit_fee = is_primary ? 1000 : 2000; // Set fee based on bank type
        cout << "Please insert the fee. After inserting, enter the amount of the inserted.\nThe fee: " << deposit_fee;
        cin >> inserted_fee;

        if (deposit_fee == inserted_fee) {
            for (const auto& cash : cash_deposited) {
                int denomination = cash.first; // 권종
                int count = cash.second; // 수량

                this->cash.addCash(denomination, count);
                total_deposit += denomination * count;
            }
            account->addFund(total_deposit);
            return "Deposit successful. New balance: " + to_string(account->getFund());
            //return "Successfully deposited " + to_string(total_deposit_value) + " KRW to account ID " + to_string(account_id);
        }
        return "The fee amount inserted is incorrect.";
    }
    return "Error: Account ID " + to_string(account_id) + " not found.";
}

string ATM::withdraw(int account_id, int amount, bool is_primary) {
    Account* account = primary_bank->getAccount(account_id);
    if (account == nullptr) {
        return "Error: Account ID " + to_string(account_id) + " not found.";
    }

    int withdrawal_fee = is_primary ? 1000 : 2000;
    int remaining_amount = amount + withdrawal_fee; //남은 금액 확인용

    //ATM에 충분한 금액이 있는지 확인
    if (remaining_amount > cash->getTotalAvailableCash()) {
        return "Insufficient cash available to dispense the requested amount including fees.";
    }
    //account에 충분한 금액이 있는지 확인
    if (remaining_amount > account->getFund()) {
        return "Insufficient account balance.";
    }
    //unordered_map<int, int> cash_dispensed; // Cash to be dispensed
    unordered_map<int, int> available_cash = cash->getAvailableCash();// ATM의 가용 현금을 가져오기

    // 현재 가용 현금의 키를 벡터로 복사하여 역순으로 순회
    //vector<int> denominations;
    for (const auto& pair : available_cash) {
        denominations.push_back(pair.first);
    }
    sort(denominations.rbegin(), denominations.rend()); // 내림차순 정렬

    // Withdraw cash using the largest denominations first
    for (int denomination : denominations) {
        int count = 0;

        while (remaining_amount >= denomination && available_cash[denomination] > 0) {
            count++;
            remaining_amount -= denomination; // 남은 금액 업데이트
            cash->subCash(denomination, 1); // 현금 감소
        }

        if (count > 0) {
            cash_dispensed[denomination] = count; // 출금 수량 기록
        }
    }
    //남은 금액 있는지 확인
    if (remaining_amount > 0) {
        return "withdraw error";
    }
    //수수료 차감 후 잔액 업데이트
    account->subFund(amount + withdrawal_fee);

    // Output the dispensed cash
    stringstream ss;
    ss << "Dispensed cash (excluding fee):\n";
    //인출 정보를 문자열로 저장
    for (const auto& pair : cash_dispensed) {
        ss << "KRW " << pair.first << ": " << pair.second << " bills\n";
    }
    ss << "Withdrawal fee: KRW " << withdrawal_fee << "\n";
    return ss.str();
}

string ATM::transfer() {
    return  "transfer\n";
}

string ATM::checkBalance(int account_id) {
    Account* account = primary_bank->getAccount(account_id);
    if (account) {
        return "Current balance: " + to_string(account->getFund());
    }
    return "Account not found.";
}


void ATM::printTransactionHistory(int account_id) {
    Account* account = primary_bank->getAccount(account_id);
    if (account) {
        const auto& history = account->getTransactionHistory();
        cout << "Transaction history for account " << account_id << ":\n";
        for (const auto& transaction : history) {
            cout << transaction << endl;
        }
    }
    else {
        cout << "Account not found.\n";
    }
}


bool ATM::isValidCard(const string& card_number) {
    if (type == "Single Bank ATM") {
        for (const auto& bank : banks) {
            for (const auto& account_pair : bank.getAccounts()) {
                Account* account = account_pair.second;
                // 카드 번호가 일치하는 계좌 찾기
                if (account->getAssociatedCard()->getCardNumber() == card_number) {
                    // 계좌의 은행이 주 은행인지 확인
                    if (account->getBankName() == primary_bank) {
                        return true;
                    }
                }
            }
        }
        return false;
    }
    else {
        for (const auto& bank : banks) {
            for (const auto& account_pair : bank.getAccounts()) {
                Account* account = account_pair.second;
                // 카드 번호가 일치하는 계좌 찾기
                if (account->getAssociatedCard()->getCardNumber() == card_number) {
                    return true;
                }
            }
        }
        return false;
    }
}

bool ATM::isCorrectPassword(const string& card_number, const string& password) {
    // 카드 번호로 계좌 찾기
    for (const auto& bank : banks) {
        for (const auto& account_pair : bank.getAccounts()) {
            Account* account = account_pair.second;
            // 카드 번호가 일치하는 계좌 찾기
            if (account->getAssociatedCard()->getCardNumber() == card_number) {
                if (account->getPassword() == password) {
                    return true; // 인증 성공
                }
            }
        }
    }
    return false; // 인증 실패
}

void ATM::printATMInfo() const {
    cout << "ATM Serial Number: " << serial_number << "\n"
        << "Type: " << type << "\n"
        << "Language: " << language << "\n"
        << "Available Cash: \n";

    cash->printAvailableCash();
}

void ATM::selectATM(vector<ATM>& atms) {
    int action_choice;
    do {
        cout << "Choose an ATM (0 to exit):\n";
        for (size_t i = 0; i < atms.size(); ++i) {
            cout << i + 1 << ". ATM Serial Number: " << atms[i].getSerialNumber() << "\n";
        }
        cin >> action_choice;

        if (action_choice > 0 && action_choice <= atms.size()) {
            ATM& selected_atm = atms[action_choice - 1];

            // 로그인 상태로 전환하여 인증 처리
            State* login_state = new LoginState();
            login_state->stateAction(selected_atm); // 로그인 상태에서 작업 수행
            delete login_state; // 메모리 해제
        }

    } while (action_choice != 0);

}

void ATM::setState(State* state) {
    delete currentState;
    currentState = state;
}
void ATM::performCurrentAction() {
    if (currentState) {
        currentState->stateAction(*this); // 현재 상태에서 액션 수행
    }






