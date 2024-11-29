#include "INITIALIZING.h"

void initializeSystem() {
    int choice;
    do {
        cout << "1. Create Account\n2. Create ATM\n3. Start Program\n4. Exit\nChoose an option: ";
        cin >> choice;

        if (choice == 1) {
            string bank_name, user_name, password, card_number, account_number;
            int initial_funds, card_number, password;
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
            int cash_quantity;

            cout << "Enter Initial Fund (Cash) in the following format:\n";

            cout << "Enter the number of KRW 50,000 bills: ";
            cin >> cash_quantity;
            initial_cash[50000] = cash_quantity;

            cout << "Enter the number of KRW 10,000 bills: ";
            cin >> cash_quantity;
            initial_cash[10000] = cash_quantity;

            cout << "Enter the number of KRW 5,000 bills: ";
            cin >> cash_quantity;
            initial_cash[5000] = cash_quantity;

            cout << "Enter the number of KRW 1,000 bills: ";
            cin >> cash_quantity;
            initial_cash[1000] = cash_quantity;

            // 입력된 현금을 출력하여 확인
            cout << "Initial cash setup completed. Current cash amounts:\n";
            for (const auto& cash : initial_cash) {
                cout << "KRW " << cash.first << ": " << cash.second << " bills" << endl;
            }
            //initial_cash.printAvailableCash();

            Bank* primary_bank = Bank::getOrCreateBank(bank_name, banks);

            ATM atm(primary_bank, serial_number, type, language, initial_cash, banks);
            atms.push_back(atm); // ATM 리스트에 추가
            cout << "ATM created successfully and linked to bank: " << bank_name << endl;
        }
    } while (choice != 4);
}
