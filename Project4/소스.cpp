#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <iomanip> // for std::setprecision
#include <sstream>
#include <memory> // for std::unique ??
#include <algorithm>

using namespace std;

// ��ǥ ����� �� �ݿ� �ʿ�
//language, ATM type �ݿ� �ʿ�

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
    //���� string connected_card_number; 
    Card* associated_card;
    vector<string> transaction_history; // Transaction history ??

public:
    //���� Account(Bank* bank, string user_name, int account_number, int initial_funds, string password, string card_number);
    Account(Bank* bank, string user_name, int initial_funds, string password, const string& card_number, const string& account_number);
    ~Account();

    // Methods
    string getBankName() const; // Get the bank name
    int getAccountId() const { return account_id; }// Get the account ID
    string getAccountNumber() const { return account_number; }
    //���� string getConnectedCardNumber() const { return connected_card_number; }
    void addFund(int fund); // Add funds to the account
    bool subFund(int fund); // Subtract funds from the account
    string getUserName() const { return account_user_name; }
    string getPassword() const { return account_password; } // Get the account password
    int getFund() const { return account_funds; } // Get the account balance
    Card* getAssociatedCard() const { return associated_card; } //ī�� ������
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
    //�� ATM�� ���ӵǴ� ������ cash�� �ʿ�  atm1_cash=new Cash() �̷���?
public:
    Cash() : cash_value(0) {}
    //�� ATM�� ���ӵǴ� ������ cash ���� �ʿ�
    //virtual ~Cash() {}
    int getValue() const { return cash_value; };//�� �ݾ� ��ȯ cash_value�� ������ ���������� �Ź� ������Ʈ�ؾ� ��
    //�� �������� ��Ȳ ��ȯ
    //unordered_map<int, int> getAvailableCash() const {
    unordered_map<int, int> getAvailableCash() {
        return cash_available;
    }

    int getTotalAvailableCash() const { // �� ���� ������ ����ϴ� �޼���
        int total = 0;
        for (const auto& pair : cash_available) {
            total += pair.first * pair.second; // ���� * ����
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
    string deposit(int account_id, unordered_map<int, int>& cash_deposited, bool is_primary); //�ٸ������� primaryȮ���ؾ� ��
    string withdraw(int account_id, int amount, bool is_primary);
    string transfer();
    string checkBalance(int account_id);
    string getSerialNumber() const { return serial_number; };
    void printTransactionHistory(int account_id);  // Print the transaction history of an account
    void printATMInfo() const;
    bool isValidCard(const string& card_number);
    bool isCorrectPassword(const string& card_number, const string& password);
    void startSession();//�� �ʿ����?
    void selectATM(vector<ATM>& atms);
    void admin_menu();

    void setCurrentAccount(Account* account) { current_account = account; } // ī�� ����
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
    virtual void stateAction(ATM& atm) = 0; // ���� ���� �Լ�
};

//Implementation of Cash methods
void Cash::addCash(int denomination, int count) {
    cash_available[denomination] += count;
    cash_value += denomination * count; // �� �ݾ� ������Ʈ
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
//���� Account::Account(Bank* bank, string user_name, int account_number, int initial_funds, string password, string card_number)
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
        // ������ �������� ������ ���� ����
        banks.emplace_back(bank_name);
        return &banks.back(); // ���� ������ ������ ����Ŵ
    }
    else {
        // ������ ������ ��ȯ
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
    cash->addCash(denomination, count);// �ʿ��Ѱ�?
}

void ATM::printAvailableCash() const {
    cash->printAvailableCash();
}
int ATM::getTotalAvailableCash() const {
    return cash->getTotalAvailableCash();
}

string ATM::deposit_check(int account_id, bool is_primary) {
    Account* account = primary_bank->getAccount(account_id);
    // ��ȿ�� �������� Ȯ��
    if (account) {
        int check;
        cout << "Insert the check: ";
        cin >> check;
        // ��ǥ �ݾ��� 100,000�� �̻����� Ȯ��
        if (check >= 100000) {
            int inserted_fee = 0;
            int deposit_fee = is_primary ? 1000 : 2000; // Set fee based on bank type
            cout << "Please insert the fee. After inserting, enter the amount of the inserted.\nThe fee: " << deposit_fee;
            cin >> inserted_fee;
            // ������ �°� �Է��ߴ��� Ȯ��
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
    //cout�� ���⼭ ���� ����, ���ڿ��� ��ȯ�ϱ�, ATM ���� �Լ��� action�� �ϰ� �����  �ܺο���

    if (account) {
        int inserted_fee = 0;
        int total_deposit = 0;

        int deposit_fee = is_primary ? 1000 : 2000; // Set fee based on bank type
        cout << "Please insert the fee. After inserting, enter the amount of the inserted.\nThe fee: " << deposit_fee;
        cin >> inserted_fee;

        if (deposit_fee == inserted_fee) {
            for (const auto& cash : cash_deposited) {
                int denomination = cash.first; // ����
                int count = cash.second; // ����

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
    int remaining_amount = amount + withdrawal_fee; //���� �ݾ� Ȯ�ο�

    //ATM�� ����� �ݾ��� �ִ��� Ȯ��
    if (remaining_amount > cash->getTotalAvailableCash()) {
        return "Insufficient cash available to dispense the requested amount including fees.";
    }
    //account�� ����� �ݾ��� �ִ��� Ȯ��
    if (remaining_amount > account->getFund()) {
        return "Insufficient account balance.";
    }
    //unordered_map<int, int> cash_dispensed; // Cash to be dispensed
    unordered_map<int, int> available_cash = cash->getAvailableCash();// ATM�� ���� ������ ��������

    // ���� ���� ������ Ű�� ���ͷ� �����Ͽ� �������� ��ȸ
    //vector<int> denominations;
    for (const auto& pair : available_cash) {
        denominations.push_back(pair.first);
    }
    sort(denominations.rbegin(), denominations.rend()); // �������� ����

    // Withdraw cash using the largest denominations first
    for (int denomination : denominations) {
        int count = 0;

        while (remaining_amount >= denomination && available_cash[denomination] > 0) {
            count++;
            remaining_amount -= denomination; // ���� �ݾ� ������Ʈ
            cash->subCash(denomination, 1); // ���� ����
        }

        if (count > 0) {
            cash_dispensed[denomination] = count; // ��� ���� ���
        }
    }
    //���� �ݾ� �ִ��� Ȯ��
    if (remaining_amount > 0) {
        return "withdraw error";
    }
    //������ ���� �� �ܾ� ������Ʈ
    account->subFund(amount + withdrawal_fee);

    // Output the dispensed cash
    stringstream ss;
    ss << "Dispensed cash (excluding fee):\n";
    //���� ������ ���ڿ��� ����
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
                // ī�� ��ȣ�� ��ġ�ϴ� ���� ã��
                if (account->getAssociatedCard()->getCardNumber() == card_number) {
                    // ������ ������ �� �������� Ȯ��
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
                // ī�� ��ȣ�� ��ġ�ϴ� ���� ã��
                if (account->getAssociatedCard()->getCardNumber() == card_number) {
                    return true;
                }
            }
        }
        return false;
    }
}

bool ATM::isCorrectPassword(const string& card_number, const string& password) {
    // ī�� ��ȣ�� ���� ã��
    for (const auto& bank : banks) {
        for (const auto& account_pair : bank.getAccounts()) {
            Account* account = account_pair.second;
            // ī�� ��ȣ�� ��ġ�ϴ� ���� ã��
            if (account->getAssociatedCard()->getCardNumber() == card_number) {
                if (account->getPassword() == password) {
                    return true; // ���� ����
                }
            }
        }
    }
    return false; // ���� ����
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

            // �α��� ���·� ��ȯ�Ͽ� ���� ó��
            State* login_state = new LoginState();
            login_state->stateAction(selected_atm); // �α��� ���¿��� �۾� ����
            delete login_state; // �޸� ����
        }

    } while (action_choice != 0);

}

void ATM::setState(State* state) {
    delete currentState;
    currentState = state;
}
void ATM::performCurrentAction() {
    if (currentState) {
        currentState->stateAction(*this); // ���� ���¿��� �׼� ����
    }






