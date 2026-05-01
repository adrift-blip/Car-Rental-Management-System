#include "Invoice.h"

Invoice::Invoice(const string& rentalID,
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
                 double taxRate, int penalties)
{
    this->rentalID = rentalID;
    this->customerName = customerName;
    this->carName = carName;
    this->carID = carID;
    this->startDate = startDate;
    this->endDate = endDate;
    this->rentalDays = rentalDays;
    this->baseCost = baseCost;
    this->driverCost = driverCost;
    this->insuranceCost = insuranceCost;
    this->deliveryCost = deliveryCost;
    this->driver = hasDriver;
    this->insurance = hasInsurance;
    this->delivery = hasDelivery;
    this->discountRate = discountRate;
    this->taxRate = taxRate;
    addOnTotal = driverCost + insuranceCost + deliveryCost;
    discountAmount = (baseCost + addOnTotal) * discountRate;
    double afterDiscount = (baseCost + addOnTotal) - discountAmount;
    taxAmount = afterDiscount * taxRate;
    totalCost = afterDiscount + taxAmount;
    this->penalties = penalties;
}

string Invoice::getRentalID() const{ return rentalID; }
int Invoice::getPenalties() const{ return penalties;}
string Invoice::getCustomerName() const{ return customerName; }
string Invoice::getCarName() const{ return carName; }
string Invoice::getCarID() const { return carID; }
string Invoice::getStartDate()const { return startDate; }
string Invoice::getEndDate() const { return endDate; }
int Invoice::getRentalDays() const{ return rentalDays; }
double Invoice::getBaseCost() const{return baseCost; }
double Invoice::getDriverCost() const{return driverCost;}
double Invoice::getInsuranceCost() const{ return insuranceCost;}
double Invoice::getDeliveryCost() const {return deliveryCost;}
double Invoice::getAddOnTotal() const{return addOnTotal;}
double Invoice::getDiscountRate() const { return discountRate; }
double Invoice::getDiscountAmount() const { return discountAmount; }
double Invoice::getTaxRate() const{return taxRate;}
double Invoice::getTaxAmount() const{return taxAmount;}
double Invoice::getTotalCost() const{ return totalCost;}
bool Invoice::hasDriverService() const { return driver; }
bool Invoice::hasInsuranceService() const { return insurance; }
bool Invoice::hasDeliveryService() const { return delivery; }
string Invoice::getAddOnDetail() const {
    if (!driver && !insurance && !delivery)
        return "None";
    string detail = "";
    if (driver) {
        detail += "Local Driver";
    }
    if (insurance) {
        if (!detail.empty()) detail += ", ";
        detail += "Insurance";
    }
    if (delivery) {
        if (!detail.empty()) detail += ", ";
        detail += "City Delivery";
    }
    return detail;
}
