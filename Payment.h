#ifndef PAYMENT_H
#define PAYMENT_H
#include "inventory.h"
#include <iostream>
#include <cstring>

class Payment
{
private:
    char paymentID[20];
    char customerName[50];
    char carType[20];       // Economy / Standard / Luxury / SUV
    int  rentalDays;
    double dailyRate;
    char paymentMethod[20];

    // Feature flags
    bool gpsNavigation;
    bool childSeat;
    bool insuranceCoverage;
    bool chauffeurService;
    bool fuelPackage;

    // Extra charges
    double extraCharges;
    char   extraChargesReason[100];

    // Tax
    double taxRate;

    // Calculated
    double featuresCost;
    double subtotal;
    double taxAmount;
    double totalCost;

    // Private helpers
    void   doubleToStr(double value, char* buf, int decimals) const;
    void   intToStr(int value, char* buf) const;
    double featureRate(const char* feature) const;
    char paymentDate[15];
    char expiryDate[15];
    bool isPaid;
    int retryCount;

public:
    Payment(const char* paymentID,
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
            double taxRate);

    bool validateAmount() const;
    bool isPaymentExpired(const char* currentDate, const char* expiryDate) const;
    void retryPayment();
    char* generatePaymentId(char* buf) const;
    void displayTransactionHistory() const;
    void displayPaymentSummary() const;
    void exportToFile(const char* filename) const;
    bool loadFromFile(const char* filename);
    // Setters
    void setPaymentID(const char* id);
    void setCustomerName(const char* name);
    void setCarType(const char* type);
    void setRentalDays(int days);
    void setDailyRate(double rate);
    void setGpsNavigation(bool val);
    void setChildSeat(bool val);
    void setInsuranceCoverage(bool val);
    void setChauffeurService(bool val);
    void setFuelPackage(bool val);
    void setExtraCharges(double charges, const char* reason);
    void setTaxRate(double rate);
    void setPaymentMethod(const char* method);


    // Getters
    const char* getPaymentID()          const;
    const char* getCustomerName()       const;
    const char* getCarType()            const;
    int         getRentalDays()         const;
    double      getDailyRate()          const;
    bool        getGpsNavigation()      const;
    bool        getChildSeat()          const;
    bool        getInsuranceCoverage()  const;
    bool        getChauffeurService()   const;
    bool        getFuelPackage()        const;
    double      getExtraCharges()       const;
    const char* getExtraChargesReason() const;
    double      getTaxRate()            const;
    double      getFeaturesCost()       const;
    double      getSubtotal()           const;
    double      getTaxAmount()          const;
    double      getTotalCost()          const;

    // Core
    void calculatePayment();
    //void displayPaymentSummary() const;

    // Static utility
    static double getDailyRateByCarType(const char* carType);
    static double extractDailyRate(const Car* car, double kmPerDay = 100.0);
    void displayPayment()const;
    const char* getPaymentMethod() const;
    void generateReceipt()const;

    bool isComplete() const;
    void printOneLiner() const;
    void updateCarType(const char* newType);

};

#endif