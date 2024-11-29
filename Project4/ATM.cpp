#include "ATM.h"
using namespace std;

vector<ATM> atms;

// ATM class
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

int ATM::deposit(Account* account, unordered_map<int, int>& cash_deposited) {
    int total_deposit = 0;
    for (const auto& cash : cash_deposited) {
        int denomination = cash.first; // ����
        int count = cash.second; // ����

        this->cash->addCash(denomination, count);
        total_deposit += denomination * count;
    }
    return total_deposit;
}

bool ATM::withdrawAvailable(int remaining_amount) {
    unordered_map<int, int> available_cash = cash->getAvailableCash();// ATM�� ���� ������ ��������

    // ���� ���� ������ Ű�� ���ͷ� �����Ͽ� �������� ��ȸ
    vector<int> denominations;
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
        }
    }

    if (remaining_amount == 0)
        return true;
    else
        return false;
}

string ATM::withdraw(int remaining_amount) {
    int total_amount = remaining_amount;
    
    //unordered_map<int, int> cash_dispensed; // Cash to be dispensed
    unordered_map<int, int> available_cash = cash->getAvailableCash();// ATM�� ���� ������ ��������

    // ���� ���� ������ Ű�� ���ͷ� �����Ͽ� �������� ��ȸ
    vector<int> denominations;
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

    // Output the dispensed cash
    ostringstream oss;
    oss << "Dispensed cash (including fee):\n";
    //���� ������ ���ڿ��� ����
    for (const auto& pair : cash_dispensed) {
        oss << "KRW " << pair.first << ": " << pair.second << " bills\n";
    }
    oss << "Total Withdrawal Amount: " << total_amount << "\n";
    oss << "Withdrawal Fee: KRW " << withdrawal_fee << "\n";
    return oss.str();
}

string ATM::cashTransfer(Account* destination, int amount, int fee) {
    ostringstream oss;
    unordered_map<int, int> cash_deposited = makeCashDeposited(); // ���� �Ա� ����
    unordered_map<int, int> fee_deposited = makeFeeDeposited(fee);

    if (fee_deposited[1000] * 1000 == fee) {
        int fund_amount = deposit(destination, cash_deposited);
        destination->addFund(fund_amount);
        deposit(destination, fee_deposited);
        oss << "Cash transfer successful.";
    }
    else
        oss << "The fee amount inserted is incorrect.";
    return oss.str();
}

string ATM::accountTransfer(Account* source, Account* destination, int amount) {
    source->subFund(amount);
    destination->addFund(amount);

    return "Account transfer successful.";
}

unordered_map<int, int> ATM::makeCashDeposited() {
    unordered_map<int, int> cash_deposited; // ���� �Ա� ����
    cout << "Enter amount to deposit into account " << endl;
    cout << "Enter number of KRW 50,000 bills: ";
    cin >> cash_deposited[50000];
    cout << "Enter number of KRW 10,000 bills: ";
    cin >> cash_deposited[10000];
    cout << "Enter number of KRW 5,000 bills: ";
    cin >> cash_deposited[5000];
    cout << "Enter number of KRW 1,000 bills: ";
    cin >> cash_deposited[1000];

    return cash_deposited;
}

unordered_map<int, int> ATM::makeFeeDeposited(int fee) {
    unordered_map<int, int> fee_deposited;
    fee_deposited[50000] = 0;
    fee_deposited[10000] = 0;
    fee_deposited[5000] = 0;
    cout << "Enter the deposit fee: " << fee << endl;
    cout << "Enter number of KRW 1,000 bills: ";
    cin >> fee_deposited[1000];

    return fee_deposited;
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
                    if (account->getBankName() == primary_bank->getName()) {
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
/*
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
*/

void ATM::setState(State* state) {
    delete currentState;
    currentState = state;
}

vector<string> ATM::getTransactionHistory() {
    return this->transaction_history;
}
void ATM::recordTransactionHistory(string rec) {
    this->transaction_history.push_back(rec);
}

bool ATM::is_primary(Account* account) {
    return (primary_bank->getName() == account->getBankName());
}

bool ATM::getTransactionAvailable(bool primary) {
    if (this->type == "Multi Bank ATM")
        return true;
    else if (primary)
        return true;
    else
        return false;
}