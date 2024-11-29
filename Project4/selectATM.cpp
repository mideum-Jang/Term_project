#include "SELECTATM.h"

using namespace std;

void selectATM() {
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
            State* login_state = new LoginState(selected_atm, current_account);//�� �𸣰ڴº�
            login_state->stateAction(selected_atm); // �α��� ���¿��� �۾� ����
            delete login_state; // �޸� ����
        }

    } while (action_choice != 0);

}