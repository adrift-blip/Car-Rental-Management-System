#include "rental.h"
#include <string>
rental::rental(){
    rentalID = "N/A";
    userNameOfBooker = "N/A";
    startDate = "N/A";
    endDate = "N/A";
    carID = "N/A";
    totalCost = -1.0;
    status = "N/A";
    hasDriver = false;
    hasInsurance = false;
    hasDelivery = false;
    discountRate = 0.0;
}
rental::rental(string id, string name, string sDate, string eDate, string carID, double tc, string status, bool driver, bool insurance, bool delivery, double discountRate){
    rentalID = id;
    userNameOfBooker = name;
    startDate = sDate;
    endDate = eDate;
    this->carID = carID;
    totalCost = tc;
    this->status = status;
    this->hasDriver = driver;
    this->hasInsurance = insurance;
    this->hasDelivery = delivery;
    this->discountRate = discountRate;
}
rental::rental(const rental& other){
    rentalID = other.rentalID;
    userNameOfBooker = other.userNameOfBooker;
    startDate = other.startDate;
    endDate = other.endDate;
    carID = other.carID;
    totalCost = other.totalCost;
    status = other.status;
    hasDriver = other.hasDriver;
    hasInsurance = other.hasInsurance;
    hasDelivery = other.hasDelivery;
    discountRate = other.discountRate;
}
rental& rental::operator=(const rental &other){
    if(this != &other){
        rentalID = other.rentalID;
        userNameOfBooker = other.userNameOfBooker;
        startDate = other.startDate;
        endDate = other.endDate;
        carID = other.carID;
        totalCost = other.totalCost;
        status = other.status;
        hasDriver = other.hasDriver;
        hasInsurance = other.hasInsurance;
        hasDelivery = other.hasDelivery;
        discountRate = other.discountRate;
    }
    return *this;
}
string rental::getRentalID() const{
    return rentalID;
}
string rental::getNameOfBooker() const{
    return userNameOfBooker;
}
string rental::getStartDate() const{
    return startDate;
}
string rental::getEndDate() const{
    return endDate;
}
string rental::getCardID() const{
    return carID;
}
double rental::getRentalPrice() const{
    return totalCost;
}
string rental::getStatus() const{
    return status;
}
void rental::setStatus(string s){
    status = s;
}
rental::~rental(){}
bool rental::getHasDriver() const{
    return hasDriver;
}
bool rental::getHasInsurance() const{
    return hasInsurance;
}
bool rental::getHasDelivery() const{
    return hasDelivery;
}
double rental::getDiscountRate() const{
    return discountRate;
}
void rental::setHasDriver(bool val){
    hasDriver = val;
}
void rental::setHasInsurance(bool val){
    hasInsurance = val;
}
void rental::setHasDelivery(bool val){
    hasDelivery = val;
}
void rental::setDiscountRate(double rate){
    discountRate = rate;
}
