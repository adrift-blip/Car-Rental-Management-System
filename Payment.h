#ifndef PAYMENT_H
#define PAYMENT_H
#include <string>
using namespace std;

class Payment {
private:
    double totalDue;
    double amountPaid;
    double remainingBalance;
    bool isPaid;
    bool amountsMatch;
public:
    Payment(double totalDue, double amountPaid);
    double getTotalDue() const;
    double getAmountPaid() const;
    double getRemainingBalance() const;
    bool getIsPaid() const;
    bool getAmountsMatch() const;
    string getPaymentStatus() const;
};
#endif