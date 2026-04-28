#define _CRT_SECURE_NO_WARNINGS
#include "Invoice.h"
#include<string>
#include<fstream>
using namespace std;

Invoice::Invoice(const char* id,
                 const char* name,
                 const char* model,
                 const char* type,
                 int days,
                 double rate,
                 double charges,
                 const char* services,
                 double tax,
                 const char* start,
                 const char* end,
                 const Payment& pay)
    : payment(pay)
{
    strcpy(invoiceID, id);
    strcpy(customerName, name);
    strcpy(carModel, model);
    strcpy(carType, type);

    rentalDays = days;
    dailyRate = rate;
    additionalCharges = charges;
    strcpy(additionalServicesDetail, services);

    taxRate = tax;
    strcpy(rentalStartDate, start);
    strcpy(rentalEndDate, end);
    cancelled = false;
    discountAmount = 0.0;
    totalPayable = 0;
}

void Invoice::intToStr(int value, char* buf) const
{
    sprintf(buf, "%d", value);
}

void Invoice::doubleToStr(double value, char* buf, int) const
{
    sprintf(buf, "%.2f", value);
}

void Invoice::calculateTotal()
{
    double base = rentalDays * dailyRate;
    double subtotal = base + additionalCharges;
    double tax = subtotal * taxRate;
    totalPayable = subtotal + tax;
}

void Invoice::displayInvoice() const
{
    cout << "\n=========== INVOICE ==========\n";
    cout << "Invoice ID : " << invoiceID << endl;
    cout << "Customer   : " << customerName << endl;
    cout << "Car Model  : " << carModel << endl;
    cout << "Car Type   : " << carType << endl;
    cout << "Start Date : " << rentalStartDate << endl;
    cout << "End Date   : " << rentalEndDate << endl;
    cout << "Total Pay  : " << totalPayable << endl;
    cout << "==============================\n";

    payment.displayPayment();
}
void Invoice::generateReceipt() const
{
    cout << "\n======== FINAL RECEIPT ========\n";
    cout << "Invoice ID : " << invoiceID << endl;
    cout << "Customer   : " << customerName << endl;
    cout << "Total Bill : " << totalPayable << endl;
    cout << "Payment    : " << payment.getPaymentMethod() << endl;
    cout << "==============================\n";

    payment.generateReceipt();
}
bool Invoice::isOverdue(const char* currentDate) const {
    // simple string comparison - YYYY-MM-DD format
    return strcmp(currentDate, rentalEndDate) > 0;
}

void Invoice::applyDiscount(double percent) {
    if (percent < 0 || percent > 100) {
        cout << "Error: Invalid discount percent!" << endl;
        return;
    }
    discountAmount = totalPayable * (percent / 100.0);
    totalPayable -= discountAmount;
    cout << "Discount applied: $" << discountAmount << endl;
}

void Invoice::applyLateFee(double feePerDay) {
    if (feePerDay < 0) {
        cout << "Error: Late fee cannot be negative!" << endl;
        return;
    }
    totalPayable += feePerDay;
    cout << "Late fee applied: $" << feePerDay << endl;
}

void Invoice::cancelInvoice() {
    if (cancelled) {
        cout << "Invoice already cancelled!" << endl;
        return;
    }
    cancelled = true;
    totalPayable = 0.0;
    cout << "Invoice " << invoiceID << " cancelled!" << endl;
}

bool Invoice::isCancelled() const
{
    return cancelled;
}

void Invoice::displayPaymentStatus() const
{
    cout << "========== PAYMENT STATUS ==========" << endl;
    cout << "Invoice ID : " << invoiceID << endl;
    cout << "Customer   : " << customerName << endl;
    if (cancelled) {
        cout << "Status     : CANCELLED" << endl;
    }
    else if (totalPayable <= 0) {
        cout << "Status     : PAID" << endl;
    }
    else {
        cout << "Status     : PENDING" << endl;
        cout << "Amount Due : $" << totalPayable << endl;
    }
    cout << "====================================" << endl;
}
void Invoice::exportToFile(const char* filename) const {
    ofstream file(filename);
    if (file.is_open()) {
        file << "invoiceID|" << invoiceID << "\n";
        file << "customerName|" << customerName << "\n";
        file << "carModel|" << carModel << "\n";
        file << "carType|" << carType << "\n";
        file << "rentalDays|" << rentalDays << "\n";
        file << "dailyRate|" << dailyRate << "\n";
        file << "additionalCharges|" << additionalCharges << "\n";
        file << "additionalServices|" << additionalServicesDetail << "\n";
        file << "taxRate|" << taxRate << "\n";
        file << "totalPayable|" << totalPayable << "\n";
        file << "rentalStartDate|" << rentalStartDate << "\n";
        file << "rentalEndDate|" << rentalEndDate << "\n";
        file << "cancelled|" << cancelled << "\n";
        file << "discountAmount|" << discountAmount << "\n";
        file.close();
        cout << "Invoice exported to " << filename << "!" << endl;
    }
    else {
        cout << "Error: Could not open file!" << endl;
    }
}

bool Invoice::loadFromFile(const char* filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: File not found!" << endl;
        return false;
    }
    string line;
    while (getline(file, line)) {
        size_t pos = line.find('|');
        if (pos == string::npos) continue;

        string key = line.substr(0, pos);
        string val = line.substr(pos + 1);

        if (key == "invoiceID")
            strncpy(invoiceID, val.c_str(), 19);
        else if (key == "customerName")
            strncpy(customerName, val.c_str(), 49);
        else if (key == "carModel")
            strncpy(carModel, val.c_str(), 49);
        else if (key == "carType")
            strncpy(carType, val.c_str(), 19);
        else if (key == "rentalDays")
            rentalDays = stoi(val);
        else if (key == "dailyRate")
            dailyRate = stod(val);
        else if (key == "additionalCharges")
            additionalCharges = stod(val);
        else if (key == "additionalServices")
            strncpy(additionalServicesDetail, val.c_str(), 149);
        else if (key == "taxRate")
            taxRate = stod(val);
        else if (key == "totalPayable")
            totalPayable = stod(val);
        else if (key == "rentalStartDate")
            strncpy(rentalStartDate, val.c_str(), 14);
        else if (key == "rentalEndDate")
            strncpy(rentalEndDate, val.c_str(), 14);
        else if (key == "cancelled")
            cancelled = stoi(val);
        else if (key == "discountAmount")
            discountAmount = stod(val);
    }
    file.close();
    cout << "Invoice loaded from " << filename << "!" << endl;
    return true;
}bool Invoice::isDiscountApplied() const {
    return discountAmount > 0.0;
}

int Invoice::calculateLateDays(const char* currentDate) const {
    // YYYY-MM-DD format compare karo
    if (strcmp(currentDate, rentalEndDate) <= 0)
        return 0;

    // simple day difference - last 2 chars se day nikalo
    int currentDay = (currentDate[8] - '0') * 10 + (currentDate[9] - '0');
    int endDay = (rentalEndDate[8] - '0') * 10 + (rentalEndDate[9] - '0');
    int diff = currentDay - endDay;
    return diff > 0 ? diff : 0;
}

void Invoice::displayFinalAmount() const {
    char buf[30];
    cout << "======== FINAL AMOUNT ========" << endl;
    cout << "Customer      : " << customerName << endl;
    doubleToStr(totalPayable, buf, 2);
    cout << "Total Payable : $" << buf << endl;
    if (discountAmount > 0) {
        doubleToStr(discountAmount, buf, 2);
        cout << "Discount      : -$" << buf << endl;
        doubleToStr(totalPayable - discountAmount, buf, 2);
        cout << "After Discount: $" << buf << endl;
    }
    cout << "Status        : " << (cancelled ? "CANCELLED" : "ACTIVE") << endl;
    cout << "==============================" << endl;
}

bool Invoice::verifyPaymentMatch() const {
    char buf1[30], buf2[30];
    doubleToStr(totalPayable, buf1, 2);
    doubleToStr(payment.getTotalCost(), buf2, 2);
    bool match = strcmp(buf1, buf2) == 0;
    if (!match)
        cout << "Warning: Invoice & Payment amounts do not match!" << endl;
    return match;
}

double Invoice::getRemainingAmount() const {
    if (cancelled) return 0.0;
    return totalPayable - discountAmount;
}