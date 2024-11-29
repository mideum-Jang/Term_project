#include "CASH.h"

using namespace std;

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