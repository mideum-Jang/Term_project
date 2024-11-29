#ifndef CASHKRW_H
#define CASHKRW_H

#include "CASH.h"

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

#endif CASHKRW_H