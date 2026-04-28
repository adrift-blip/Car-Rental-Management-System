#define _CRT_SECURE_NO_WARNINGS
#ifndef INVOICE_H
#define INVOICE_H

#include <iostream>
#include <cstring>
#include "Payment.h"

class Invoice
{
private:
    char invoiceID[20];
    char customerName[50];
    char carModel[50];
    char carType[20];

    int rentalDays;
    double dailyRate;
    double additionalCharges;

    char additionalServicesDetail[150];

    double taxRate;
    double totalPayable;

    char rentalStartDate[15];
    char rentalEndDate[15];
    bool cancelled;
    double discountAmount;
    Payment payment;

    void intToStr(int value, char* buf) const;
    void doubleToStr(double value, char* buf, int decimals) const;

public:
    Invoice(const char* id,
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
            const Payment& pay);

    void calculateTotal();
    void displayInvoice() const;
    void generateReceipt() const;
    bool isOverdue(const char* currentDate) const;
    void applyDiscount(double percent);
    void applyLateFee(double feePerDay);
    void cancelInvoice();
    bool isCancelled() const;
    void displayPaymentStatus() const;
    void exportToFile(const char* filename) const;
    bool loadFromFile(const char* filename);
    bool isDiscountApplied() const;
    // bool isDiscountApplied() const;
    //int calculateLateDays(const char* currentDate) const;

    int calculateLateDays(const char* currentDate) const;
    void displayFinalAmount() const;
    bool verifyPaymentMatch() const;
    double getRemainingAmount() const;
};

#endif#pragma once
