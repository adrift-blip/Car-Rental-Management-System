#ifndef INVOICE_H
#define INVOICE_H

#include <string>
using namespace std;

class Invoice {
private:
    string rentalID;
    string customerName;
    string carName;
    string carID;
    string startDate;
    string endDate;
    int rentalDays;
    double baseCost;
    double driverCost;
    double insuranceCost;
    double deliveryCost;
    double addOnTotal;
    double discountRate;
    double discountAmount;
    double taxRate;
    double taxAmount;
    double totalCost;
    bool driver;
    bool insurance;
    bool delivery;
public:
    Invoice(const string& rentalID,
            const string& customerName,
            const string& carName,
            const string& carID,
            const string& startDate,
            const string& endDate,
            int rentalDays,
            double baseCost,
            double driverCost,
            double insuranceCost,
            double deliveryCost,
            bool hasDriver,
            bool hasInsurance,
            bool hasDelivery,
            double discountRate,
            double taxRate);

    string getRentalID() const;
    string getCustomerName()const;
    string getCarName()const;
    string getCarID() const;
    string getStartDate() const;
    string getEndDate() const;
    int getRentalDays()const;
    double getBaseCost() const;
    double getDriverCost() const;
    double getInsuranceCost() const;
    double getDeliveryCost() const;
    double getAddOnTotal() const;
    double getDiscountRate() const;
    double getDiscountAmount() const;
    double getTaxRate() const;
    double getTaxAmount() const;
    double getTotalCost() const;

    bool hasDriverService()const;
    bool hasInsuranceService() const;
    bool hasDeliveryService() const;
    string getAddOnDetail() const;
};

#endif