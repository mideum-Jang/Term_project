#ifndef CASH_H
#define CASH_H

#include <iostream>
#include <unordered_map>

using namespace std;

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

#endif CASH_H