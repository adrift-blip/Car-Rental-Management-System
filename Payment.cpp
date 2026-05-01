#include "Payment.h"
#include <cmath>

Payment::Payment(double totalDue, double amountPaid)
{
    this->totalDue = totalDue;
    this->amountPaid = amountPaid;

    remainingBalance = totalDue-amountPaid;
    isPaid = amountPaid >= totalDue;
    amountsMatch = fabs(totalDue-amountPaid) < 0.01;
}

double Payment::getTotalDue()const { return totalDue; }
double Payment::getAmountPaid() const { return amountPaid; }
double Payment::getRemainingBalance() const { return remainingBalance; }
bool Payment::getIsPaid()const { return isPaid; }
bool Payment::getAmountsMatch() const { return amountsMatch; }
string Payment::getPaymentStatus() const {
    return isPaid ? "Paid" : "Pending";
}
