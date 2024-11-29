#ifndef CASH_H
#define CASH_H

#include <iostream>
#include <unordered_map>

using namespace std;

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

#endif CASH_H