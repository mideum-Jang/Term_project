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

            // 로그인 상태로 전환하여 인증 처리
            State* login_state = new LoginState(selected_atm, current_account);//잘 모르겠는분
            login_state->stateAction(selected_atm); // 로그인 상태에서 작업 수행
            delete login_state; // 메모리 해제
        }

    } while (action_choice != 0);

}