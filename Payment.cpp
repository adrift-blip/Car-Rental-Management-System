#define _CRT_SECURE_NO_WARNINGS
#include "Payment.h"
#include <cstring>
#include <cmath>
#include<fstream>
#include<string>
#include <iostream>
using namespace std;

// Private Helpers

void Payment::doubleToStr(double value, char* buf, int decimals) const
{
    int intPart = (int)value;
    double fracPart = value - intPart;

    int i = 0;
    if (intPart == 0)
    {
        buf[i++] = '0';
    }
    else\
        {
            char temp[20];
            int t = 0;
            while (intPart > 0)
            {
                temp[t++] = '0' + (intPart % 10);
                intPart /= 10;
            }
            for (int j = t - 1; j >= 0; j--)
            {
                buf[i++] = temp[j];
            }
        }

    if (decimals > 0) {
        buf[i++] = '.';
        for (int d = 0; d < decimals; d++)
        {
            fracPart *= 10;
            buf[i++] = '0' + (int)fracPart;
            fracPart -= (int)fracPart;
        }
    }
    buf[i] = '\0';
}

void Payment::intToStr(int value, char* buf) const
{
    if (value == 0)
    {
        buf[0] = '0';
        buf[1] = '\0';
        return;
    }
    char temp[20];
    int i = 0, t = 0;
    while (value > 0)
    {
        temp[t++] = '0' + (value % 10);
        value /= 10;
    }
    for (int j = t - 1; j >= 0; j--)
        buf[i++] = temp[j];
    buf[i] = '\0';
}

double Payment::featureRate(const char* feature) const {
    if (strcmp(feature, "gps") == 0)
    {
        return 5.0;
    }
    if (strcmp(feature, "childSeat") == 0)
    {
        return 8.0;
    }
    if (strcmp(feature, "insurance") == 0)
    {
        return 15.0;
    }
    if (strcmp(feature, "chauffeur") == 0)
    {
        return 50.0;
    }
    if (strcmp(feature, "fuel") == 0)
    {
        return 20.0;
    }
    return 0.0;
}

//  Constructor

Payment::Payment(const char* paymentID,
                 const char* customerName,
                 const char* carType,
                 int rentalDays,
                 double dailyRate,
                 bool gpsNavigation,
                 bool childSeat,
                 bool insuranceCoverage,
                 bool chauffeurService,
                 bool fuelPackage,
                 double extraCharges,
                 const char* extraChargesReason,
                 double taxRate)
{
    strncpy(this->paymentID, paymentID, 19);
    this->paymentID[19] = '\0';

    strncpy(this->paymentMethod, "Cash", 19);
    this->paymentMethod[19] = '\0';

    strncpy(this->customerName, customerName, 49);
    this->customerName[49] = '\0';

    strncpy(this->carType, carType, 19);
    this->carType[19] = '\0';

    strncpy(this->extraChargesReason, extraChargesReason, 99);
    this->extraChargesReason[99] = '\0';

    this->rentalDays = rentalDays;
    this->dailyRate = dailyRate;
    this->gpsNavigation = gpsNavigation;
    this->childSeat = childSeat;
    this->insuranceCoverage = insuranceCoverage;
    this->chauffeurService = chauffeurService;
    this->fuelPackage = fuelPackage;
    this->extraCharges = extraCharges;
    this->taxRate = taxRate;

    this->dailyRate = 0.0;  // This ALWAYS runs, overwriting whatever you passed in!
    this->featuresCost = 0.0;
    this->subtotal = 0.0;
    this->taxAmount = 0.0;
    this->totalCost = 0.0;
    isPaid = false;
    retryCount = 0;
    strcpy(paymentDate, "2025-01-01");
    strcpy(expiryDate, "2025-12-31");
    // Edge case checks
    if (this->rentalDays < 0)
    {
        this->rentalDays = 0;
    }
    {
        this->dailyRate = 0.0;
    }
    if (this->extraCharges < 0)
    {
        this->extraCharges = 0.0;
    }
    {
        this->taxRate = 0.0;
    }
    if (this->dailyRate < 0)
    {
        this->dailyRate = 0.0;
    }
    if (this->taxRate < 0)        // <-- add the if
    {
        this->taxRate = 0.0;
    }
    calculatePayment();
}

// Setters

void Payment::setPaymentID(const char* id)
{
    if (id)
    {
        strncpy(paymentID, id, 19);
        paymentID[19] = '\0';
    }
}

void Payment::setCustomerName(const char* name)
{
    if (name)
    {
        strncpy(customerName, name, 49);
        customerName[49] = '\0';
    }
}

void Payment::setCarType(const char* type)
{
    if (type)
    {
        strncpy(carType, type, 19);
        carType[19] = '\0';
    }
}

void Payment::setRentalDays(int days)
{
    if (days >= 0)
    {
        rentalDays = days;
    }
    else
    {
        cout << "Error: Rental days cannot be negative!" << endl;
    }
}

void Payment::setDailyRate(double rate)
{
    if (rate >= 0)
    {
        dailyRate = rate;
    }
    else
    {
        cout << "Error: Daily rate cannot be negative!" << endl;
    }
}

void Payment::setGpsNavigation(bool val)
{
    gpsNavigation = val;
}
void Payment::setChildSeat(bool val)
{
    childSeat = val;
}
void Payment::setInsuranceCoverage(bool val)
{
    insuranceCoverage = val;
}
void Payment::setChauffeurService(bool val)
{
    chauffeurService = val;
}
void Payment::setFuelPackage(bool val)
{
    fuelPackage = val;
}

double Payment::extractDailyRate(const Car* car, double kmPerDay)
{
    if (!car) return 0.0;
    return car->getPrice() * kmPerDay;
}

void Payment::setExtraCharges(double charges, const char* reason)
{
    if (charges >= 0)
    {
        extraCharges = charges;
        if (reason)
        {
            strncpy(extraChargesReason, reason, 99);
            extraChargesReason[99] = '\0';
        }
    }
    else {
        cout << "Error: Extra charges cannot be negative!" << endl;
    }
}

void Payment::setPaymentMethod(const char* method)
{
    if (method)
    {
        strncpy(paymentMethod, method, 19);
        paymentMethod[19] = '\0';
    }
}

void Payment::setTaxRate(double rate)
{
    if (rate >= 0)
    {
        taxRate = rate;
    }
    else
    {
        cout << "Error: Tax rate cannot be negative!" << endl;
    }
}

// Getters

const char* Payment::getPaymentID()          const
{
    return paymentID;
}
const char* Payment::getCustomerName()       const
{
    return customerName;
}
const char* Payment::getCarType()            const
{
    return carType;
}
int         Payment::getRentalDays()         const
{
    return rentalDays;
}
double      Payment::getDailyRate()          const
{
    return dailyRate;
}
bool        Payment::getGpsNavigation()      const
{
    return gpsNavigation;
}
bool        Payment::getChildSeat()          const
{
    return childSeat;
}
bool        Payment::getInsuranceCoverage()  const
{
    return insuranceCoverage;
}
bool        Payment::getChauffeurService()   const
{
    return chauffeurService;
}
bool        Payment::getFuelPackage()        const
{
    return fuelPackage;
}
double      Payment::getExtraCharges()       const
{
    return extraCharges;
}
const char* Payment::getExtraChargesReason() const
{
    return extraChargesReason;
}
double      Payment::getTaxRate()            const
{
    return taxRate;
}
double      Payment::getFeaturesCost()       const
{
    return featuresCost;
}
double      Payment::getSubtotal()           const
{
    return subtotal;
}
double      Payment::getTaxAmount()          const
{
    return taxAmount;
}
double      Payment::getTotalCost()          const
{
    return totalCost;
}

// Core

void Payment::calculatePayment()
{
    // Edge case: 0 days
    if (rentalDays == 0
        ) {
        featuresCost = subtotal = taxAmount = totalCost = 0.0;
        return;
    }

    // Features cost per day
    featuresCost = 0.0;
    if (gpsNavigation)     featuresCost += featureRate("gps");
    if (childSeat)         featuresCost += featureRate("childSeat");
    if (insuranceCoverage) featuresCost += featureRate("insurance");
    if (chauffeurService)  featuresCost += featureRate("chauffeur");
    if (fuelPackage)       featuresCost += featureRate("fuel");

    featuresCost *= rentalDays;

    subtotal = (dailyRate * rentalDays) + featuresCost + extraCharges;
    taxAmount = subtotal * (taxRate / 100.0);
    totalCost = subtotal + taxAmount;
}

const char* Payment::getPaymentMethod() const
{
    return paymentMethod;
}

void Payment::displayPayment() const
{
    displayPaymentSummary(); // same hai
}

void Payment::generateReceipt() const
{
    char buf[30];
    cout << "========================================" << endl;
    cout << "           PAYMENT RECEIPT              " << endl;
    cout << "========================================" << endl;
    cout << "Payment ID   : " << paymentID << endl;
    cout << "Customer     : " << customerName << endl;
    cout << "Car Type     : " << carType << endl;
    cout << "Rental Days  : " << rentalDays << endl;
    cout << "Payment Mode : " << paymentMethod << endl;
    doubleToStr(totalCost, buf, 2);
    cout << "Total Paid   : $" << buf << endl;
    cout << "========================================" << endl;
    cout << "       Thank you for choosing us!       " << endl;
    cout << "========================================" << endl;
}

void Payment::displayPaymentSummary() const
{
    char buf[30];
    cout << "========================================" << endl;
    cout << "         PAYMENT SUMMARY                " << endl;
    cout << "========================================" << endl;
    cout << "Payment ID    : " << paymentID << endl;
    cout << "Customer      : " << customerName << endl;
    cout << "Car Type      : " << carType << endl;
    cout << "Rental Days   : " << rentalDays << endl;

    doubleToStr(dailyRate, buf, 2);
    cout << "Daily Rate    : $" << buf << endl;

    cout << "----------------------------------------" << endl;
    cout << "Add-ons       :" << endl;
    cout << "  GPS         : " << (gpsNavigation ? "Yes" : "No") << endl;
    cout << "  Child Seat  : " << (childSeat ? "Yes" : "No") << endl;
    cout << "  Insurance   : " << (insuranceCoverage ? "Yes" : "No") << endl;
    cout << "  Chauffeur   : " << (chauffeurService ? "Yes" : "No") << endl;
    cout << "  Fuel Pack   : " << (fuelPackage ? "Yes" : "No") << endl;

    doubleToStr(featuresCost, buf, 2);
    cout << "Add-ons Cost  : $" << buf << endl;

    cout << "----------------------------------------" << endl;
    if (extraCharges > 0.0) {
        doubleToStr(extraCharges, buf, 2);
        cout << "Extra Charges : $" << buf << endl;
        cout << "Reason        : " << extraChargesReason << endl;
    }

    doubleToStr(subtotal, buf, 2);
    cout << "Subtotal      : $" << buf << endl;

    doubleToStr(taxRate, buf, 2);
    cout << "Tax Rate      : " << buf << "%" << endl;

    doubleToStr(taxAmount, buf, 2);
    cout << "Tax Amount    : $" << buf << endl;

    cout << "========================================" << endl;
    doubleToStr(totalCost, buf, 2);
    cout << "TOTAL COST    : $" << buf << endl;
    cout << "========================================" << endl;
}

// ── Static Utility ────────────────────────────────────────────

double Payment::getDailyRateByCarType(const char* carType)
{
    if (strcmp(carType, "Economy") == 0) return 30.0;
    if (strcmp(carType, "Standard") == 0) return 50.0;
    if (strcmp(carType, "Luxury") == 0) return 100.0;
    if (strcmp(carType, "SUV") == 0) return 80.0;

    cout << "Error: Unknown car type! Defaulting to Economy rate." << endl;
    return 30.0;
}
bool Payment::validateAmount() const {
    if (totalCost <= 0) {
        cout << "Error: Total amount must be greater than 0!" << endl;
        return false;
    }
    if (rentalDays <= 0) {
        cout << "Error: Rental days must be greater than 0!" << endl;
        return false;
    }
    if (dailyRate <= 0) {
        cout << "Error: Daily rate must be greater than 0!" << endl;
        return false;
    }
    cout << "Payment validated successfully!" << endl;
    return true;
}

bool Payment::isPaymentExpired(const char* currentDate,
                               const char* expDate) const {
    return strcmp(currentDate, expDate) > 0;
}

void Payment::retryPayment() {
    if (isPaid) {
        cout << "Error: Payment already completed!" << endl;
        return;
    }
    if (retryCount >= 3) {
        cout << "Error: Max retry limit reached! Contact bank." << endl;
        return;
    }
    retryCount++;
    cout << "Retrying payment... Attempt: " << retryCount << endl;
    if (validateAmount()) {
        isPaid = true;
        cout << "Payment successful on retry " << retryCount << "!" << endl;
    }
}

char* Payment::generatePaymentId(char* buf) const {
    // PAY + customerName first 3 chars + rentalDays
    strcpy(buf, "PAY");
    int i = 3;
    for (int j = 0; j < 3 && customerName[j] != '\0'; j++)
        buf[i++] = customerName[j];
    char dayBuf[10];
    intToStr(rentalDays, dayBuf);
    strcpy(buf + i, dayBuf);
    return buf;
}

void Payment::displayTransactionHistory() const {
    char buf[30];
    cout << "====== TRANSACTION HISTORY ======" << endl;
    cout << "Payment ID   : " << paymentID << endl;
    cout << "Customer     : " << customerName << endl;
    cout << "Car Type     : " << carType << endl;
    cout << "Rental Days  : " << rentalDays << endl;
    cout << "Payment Date : " << paymentDate << endl;
    cout << "Method       : " << paymentMethod << endl;
    doubleToStr(totalCost, buf, 2);
    cout << "Total Paid   : $" << buf << endl;
    cout << "Status       : " << (isPaid ? "PAID" : "PENDING") << endl;
    cout << "Retry Count  : " << retryCount << endl;
    cout << "=================================" << endl;
}

void Payment::exportToFile(const char* filename) const {
    char buf[30];
    ofstream file(filename);
    if (file.is_open()) {
        file << "paymentID|" << paymentID << "\n";
        file << "customerName|" << customerName << "\n";
        file << "carType|" << carType << "\n";
        file << "rentalDays|" << rentalDays << "\n";
        file << "dailyRate|" << dailyRate << "\n";
        file << "gps|" << gpsNavigation << "\n";
        file << "childSeat|" << childSeat << "\n";
        file << "insurance|" << insuranceCoverage << "\n";
        file << "chauffeur|" << chauffeurService << "\n";
        file << "fuel|" << fuelPackage << "\n";
        file << "extraCharges|" << extraCharges << "\n";
        file << "extraReason|" << extraChargesReason << "\n";
        file << "taxRate|" << taxRate << "\n";
        file << "totalCost|" << totalCost << "\n";
        file << "isPaid|" << isPaid << "\n";
        file << "paymentMethod|" << paymentMethod << "\n";
        file.close();
        cout << "Payment exported to " << filename << "!" << endl;
    }
    else {
        cout << "Error: Could not open file!" << endl;
    }
}

bool Payment::loadFromFile(const char* filename) {
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

        if (key == "paymentID")    strncpy(paymentID, val.c_str(), 19);
        else if (key == "customerName") strncpy(customerName, val.c_str(), 49);
        else if (key == "carType") strncpy(carType, val.c_str(), 19);
        else if (key == "rentalDays")   rentalDays = stoi(val);
        else if (key == "dailyRate")    dailyRate = stod(val);
        else if (key == "gps")          gpsNavigation = stoi(val);
        else if (key == "childSeat")    childSeat = stoi(val);
        else if (key == "insurance")    insuranceCoverage = stoi(val);
        else if (key == "chauffeur")    chauffeurService = stoi(val);
        else if (key == "fuel")         fuelPackage = stoi(val);
        else if (key == "extraCharges") extraCharges = stod(val);
        else if (key == "extraReason")
            strncpy(extraChargesReason, val.c_str(), 99);
        else if (key == "taxRate")      taxRate = stod(val);
        else if (key == "isPaid")       isPaid = stoi(val);
        else if (key == "paymentMethod")
            strncpy(paymentMethod, val.c_str(), 19);
    }
    file.close();
    calculatePayment();
    cout << "Payment loaded from " << filename << "!" << endl;
    return true;
}
bool Payment::isComplete() const {
    if (!isPaid) {
        cout << "Payment not completed!" << endl;
        return false;
    }
    if (totalCost <= 0) {
        cout << "Invalid total cost!" << endl;
        return false;
    }
    if (rentalDays <= 0) {
        cout << "Invalid rental days!" << endl;
        return false;
    }
    if (strlen(customerName) == 0) {
        cout << "Customer name missing!" << endl;
        return false;
    }
    if (strlen(paymentMethod) == 0) {
        cout << "Payment method missing!" << endl;
        return false;
    }
    cout << "Payment is complete!" << endl;
    return true;
}

void Payment::printOneLiner() const {
    char buf[30];
    doubleToStr(totalCost, buf, 2);
    cout << "[" << paymentID << "] "
         << customerName << " | "
         << carType << " | "
         << rentalDays << " days | $"
         << buf << " | "
         << (isPaid ? "PAID" : "PENDING")
         << endl;
}

void Payment::updateCarType(const char* newType) {
    if (!newType || strlen(newType) == 0) {
        cout << "Error: Invalid car type!" << endl;
        return;
    }
    strncpy(carType, newType, 19);
    carType[19] = '\0';

    // rate bhi update karo
    dailyRate = getDailyRateByCarType(carType);

    // recalculate karo
    calculatePayment();

    cout << "Car type updated to: " << carType << endl;
}