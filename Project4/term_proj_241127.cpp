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
    unordered_map<int, Account*> accounts; // Map of accounts

public:
    Bank(const string& bank_name) : name(bank_name) {}
    string getName() const { return name; }
    Account* createAccount(string user_name, int initial_funds, string password, const string& card_number, const string& accout_number);// Create a new account
    Account* getAccount(int account_id); // Get an account by ID
    const unordered_map<int, Account*>& getAccounts() const { return accounts; }
    ~Bank();// Destructor to clean up accounts
    static Bank* getOrCreateBank(const string& bank_name, vector<Bank>& banks);
};

// Cash class
class Cash {
private:
    unordered_map<int, int> cash_available;
    int cash_value;
    //각 ATM에 종속되는 별개의 cash가 필요
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
};class KRW_10000 : public Cash {
public:
    KRW_10000() {
        addCash(10000, 0);
    }
};class KRW_50000 : public Cash {
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
    Cash cash;
    unordered_map<int, int> cash_dispensed;
    int withdrawal_fee;
    int remaining_amount;
    unordered_map<int, int> available_cash;
    vector<int> denominations;
    int amount;
    vector<Bank>& banks;

public:
    //ATM(Bank* bank, string serial_number, string type, string language, const unordered_map<int, int>& initial_cash);
    ATM(Bank* bank, const string& serial_number, const string& type, const string& language, const unordered_map<int, int>& initial_cash, vector<Bank>& banks_ref);


    void addCash(int denomination, int count);
    void printAvailableCash() const;
    int getTotalAvailableCash() const;
    //int getAvailableCash();

    Bank* getBank();
    string deposit(int account_id, unordered_map<int, int>& cash_deposited, bool is_primary); //다른데서도 primary확인해야 함
    string withdraw(int account_id, int amount, bool is_primary);
    string transfer();
    string checkBalance(int account_id);
    string getSerialNumber() const { return serial_number; };
    void printTransactionHistory(int account_id);  // Print the transaction history of an account
    void printATMInfo() const;
    bool authorizeUser(const string& card_number, const string& password);
    //bool authorizeAccount(); //?? 계좌 비번 체크도 필요 
    //vector<Account> accessableAccounts; //?? 인증된 account list필요
    void startSession();
};

Bank* ATM::getBank() {
    return primary_bank;
}



class Card {
private:
    string card_number;
    Account* card_account;
    Bank* card_bank;
    bool card_admin=false;

public:
    //Card(const string& number) : card_number(number) {}
    string getCardNumber() const { return card_number; } // 4 dgits
    Card(Account* account, Bank* bank, string card_number);
    ~Card();
    string getBankname() { return card_bank->getName(); };
    Account* getAccount() { return card_account; };
    bool isAdmin() {return card_admin; }; //admin card number 9999 지정, admin 인식 시 다음 action 구현하기
};



Card::Card(Account* account, Bank* bank, string card_number) {
    card_account = account;
    card_bank = bank;
    this->card_number = card_number;
}

Card::~Card() {}

class State {
protected:
    Account* state_account;
    ATM* atm;
public:
    State(Account* account, ATM* atm) : state_account(account), atm(atm) {}
    virtual ~State() {}
    virtual void stateAction(); // ATM 동작을 처리
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
    associated_card = new Card(this, bank, card_number); //bool admin은??
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

// ?? Get the transaction history //session종료 시 출력?
const vector<string>& Account::getTransactionHistory() const {
    return transaction_history;
}






// Implementation of Bank methods
Account* Bank::createAccount(string user_name, int initial_funds, string password, const string& card_number, const string& account_number) {
    Account* newAccount = new Account(this, user_name, initial_funds, password, card_number, account_number);
    accounts[newAccount->getAccountId()] = newAccount; //account id로 account 저장
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
//ATM::ATM(Bank* bank, string serial_number, string type, string language, const unordered_map<int, int>& initial_cash)
ATM::ATM(Bank* bank, const string& serial_number, const string& type, const string& language, const unordered_map<int, int>& initial_cash, vector<Bank>& banks_ref)
    : primary_bank(bank), serial_number(serial_number), type(type), language(language), banks(banks_ref){
    for (const auto& pair : initial_cash) {
        cash.addCash(pair.first, pair.second);
        //각 ATM에 종속되는 별개의 cash 생성 필요
    }
}

void ATM::addCash(int denomination, int count) {
    cash.addCash(denomination, count);// 필요한가?
}
void ATM::printAvailableCash() const {
    cash.printAvailableCash();
}
int ATM::getTotalAvailableCash() const {
    return cash.getTotalAvailableCash();
}


string ATM::deposit(int account_id, unordered_map<int, int>& cash_deposited, bool is_primary) {
    Account* account = primary_bank->getAccount(account_id);

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
            return "Deposit successful. New balance: " + to_string(account->getTransactionHistory().size());
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
    if (remaining_amount > cash.getTotalAvailableCash()) {
        return "Insufficient cash available to dispense the requested amount including fees.";
    }
    //account에 충분한 금액이 있는지 확인
    if (remaining_amount > account->getFund()) {
        return "Insufficient account balance.";
    }
    //unordered_map<int, int> cash_dispensed; // Cash to be dispensed
    unordered_map<int, int> available_cash = cash.getAvailableCash();// ATM의 가용 현금을 가져오기

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
            cash.subCash(denomination, 1); // 현금 감소
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

bool ATM::authorizeUser(const string & card_number, const string & password) {
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

    cash.printAvailableCash();
    //class Cash 내부 하위 함수로 출력 구현하기
}


//state 메서드 구현
void State::stateAction() {
    string inserted_card_number;
    string password;

    // 카드 입력 및 사용자 인증
    cout << "Please insert your card (Enter card number): ";
    cin >> inserted_card_number;

    // 사용자 인증
    int attempt = 0;
    while (attempt < 3) {
        cout << "Enter password: ";
        cin >> password;
        if (atm->authorizeUser(inserted_card_number, password)) {
            cout << "Authorization successful.\n";
            break; // 인증 성공
        }
        else {
            cout << "Wrong password. Try again.\n";
            attempt++;
        }
    }

    if (attempt == 3) {
        cout << "Too many failed attempts. Session terminated.\n";
        return; // 인증 실패 시 세션 종료
    }

    // 사용자 인증 후 세션 진행
    int action_choice;
    do {
        cout << "Select action:\n";
        cout << "1. Deposit\n";
        cout << "2. Withdraw\n";
        cout << "3. Transfer\n";
        cout << "4. / (Display Account/ATM Snapshot)\n";
        cout << "5. Exit Session\n";
        cin >> action_choice;

        switch (action_choice) {
        case 1: { // Deposit
            int account_id;
            cout << "Enter account ID for deposit: ";
            cin >> account_id;

            unordered_map<int, int> cash_deposited; // 현금 입금 내역
            cout << "Enter number of KRW 50,000 bills: ";
            cin >> cash_deposited[50000];
            cout << "Enter number of KRW 10,000 bills: ";
            cin >> cash_deposited[10000];
            cout << "Enter number of KRW 5,000 bills: ";
            cin >> cash_deposited[5000];
            cout << "Enter number of KRW 1,000 bills: ";
            cin >> cash_deposited[1000];

            bool is_primary = (atm->getBank()->getName() == state_account->getBankName()); // 은행 확인
            string deposit_result = atm->deposit(account_id, cash_deposited, is_primary);
            cout << deposit_result << endl;
            break;
        }
        case 2: { // Withdraw
            int amount;
            int account_id;
            cout << "Enter account ID for withdraw: ";
            cin >> account_id;
            cout << "Enter amount to withdraw: ";
            cin >> amount;

            bool is_primary = (atm->getBank()->getName() == state_account->getBankName()); // 은행 확인
            string withdraw_result = atm->withdraw(account_id, amount, is_primary);
            cout << withdraw_result << endl;
            break;
        }
        case 3: { // Transfer
            cout << "Transfer functionality not implemented yet.\n";
            break;
        }
        case 4: { // Display Account/ATM Snapshot
            atm->printATMInfo(); // ATM 정보 출력
            break;
        }
        case 5: // Exit Session
            cout << "Exiting session.\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }

    } while (action_choice != 5);
}


void initializeSystem(vector<Bank>&banks, vector<ATM>&atms, vector<Account>&accounts) {
    int choice;
    do {
        cout << "1. Create Account\n2. Create ATM\n3. Start Program\n4. Exit\nChoose an option: ";
        cin >> choice;

        if (choice == 1) {
            string bank_name, user_name, password, card_number, account_number;
            int initial_funds;
            cout << "Enter Bank Name: ";
            cin >> bank_name;
            cout << "Enter User Name: ";
            cin >> user_name;
            cout << "Enter Account Number (12 digits): ";
            cin >> account_number; // 계좌 번호 입력 받기
            cout << "Enter Available Funds (KRW): ";
            cin >> initial_funds;
            cout << "Enter Connected Card Number (4 digits): ";
            cin >> card_number; // 카드 번호 입력 받기
            cout << "Enter Password (4 digits): ";
            cin >> password;

            // Create bank if it doesn't exist
            Bank* bank = Bank::getOrCreateBank(bank_name, banks);

            //Account new_account(account_id) = 
            //bank.createAccount(user_name, initial_funds, password, card_number, account_number); // 카드와 계좌 번호 전달
            //accounts.push_back(account);
            cout << "Account created successfully.\n";
        }



        else if (choice == 2) {
            string bank_name, serial_number, type, language;
            int type_int, language_int;
            cout << "Enter primary bank name: ";
            cin >> bank_name;
            cout << "Enter ATM Serial Number (6 digits): ";
            cin >> serial_number;
            cout << "Enter ATM Type (1. Single Bank ATM / 2. Multi Bank ATM): ";
            cin >> type_int;
            type = (type_int == 1) ? "Single Bank ATM" : "Multi Bank ATM";
            cout << "Enter Language (1. Unilingual / 2. Bilingual): ";
            cin >> language_int;
            language = (language_int == 1) ? "Unilingual" : "Bilingual";


            unordered_map<int, int> initial_cash; // {denomination: quantity}
            int cash_amount, cash_quantity;
            cout << "Enter Initial Fund (Cash) in the format 'KRW XXX: YYY bills' (type 'done' to finish):" << endl;
            while (true) {
                string input;
                cout << "Input: ";
                cin >> input;
                if (input == "done") break;

                size_t colon_pos = input.find(':');
                if (colon_pos != string::npos) {
                    string denomination_str = input.substr(3, colon_pos - 3); // Skip 'KRW '
                    string quantity_str = input.substr(colon_pos + 1);
                    cash_amount = stoi(denomination_str);
                    cash_quantity = stoi(quantity_str);
                    initial_cash[cash_amount] = cash_quantity;
                }
            }
            //// 은행 확인 및 생성
            //auto it = find_if(banks.begin(), banks.end(), [&bank_name](const Bank& bank) {
            //    return bank.getName() == bank_name;
            //    });

            //Bank* bank;
            //if (it == banks.end()) {
            //    // 은행이 존재하지 않으면 새로 생성
            //    banks.emplace_back(bank_name);
            //    bank = &banks.back(); // 새로 생성한 은행을 가리킴
            //    cout << "New bank created: " << bank_name << endl;
            //}
            //else {
            //    // 기존의 은행을 사용
            //    bank = &(*it);
            //}
            Bank* primary_bank = Bank::getOrCreateBank(bank_name, banks);

            ATM atm(primary_bank, serial_number, type, language, initial_cash, banks);
            atms.push_back(atm); // ATM 리스트에 추가
            cout << "ATM created successfully and linked to bank: " << bank_name << endl;
        }
    } while (choice != 4);
}

// Main function
int main() {
    vector<Bank> banks; // 은행 목록
    vector<ATM> atms;   // ATM 목록
    vector<Account> accounts;// 계좌 목록

    // 초기 시스템 설정
    initializeSystem(banks, atms, accounts);

    int action_choice;
    do {
        cout << "Choose an ATM (0 to exit):\n";
        for (size_t i = 0; i < atms.size(); ++i) {
            cout << i + 1 << ". ATM Serial Number: " << atms[i].getSerialNumber() << "\n";
        }
        cin >> action_choice;

        if (action_choice > 0 && action_choice <= atms.size()) {
            ATM& selected_atm = atms[action_choice - 1];
            State state(nullptr, &selected_atm); // 인증된 Account는 아직 없음.
            state.stateAction(); // 선택한 ATM 세션 시작
        }

    } while (action_choice != 0);

    return 0;
}
//KRW 1000 : 10 bills, KRW 5000 : 10 bills, KRW 10000 : 10 bills, KRW 50000 : 10 bills