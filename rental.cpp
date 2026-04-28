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
}
rental::rental(string id, string name, string sDate, string eDate, string carID, double tc, string state){
    rentalID = id;
    userNameOfBooker = name;
    startDate = sDate;
    endDate = eDate;
    this->carID = carID;
    totalCost = tc;
    status = state;
}
rental::rental(const rental& other){
    rentalID = other.rentalID;
    userNameOfBooker = other.userNameOfBooker;
    startDate = other.startDate;
    endDate = other.endDate;
    carID = other.carID;
    totalCost = other.totalCost;
    status = other.status;
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
rental::~rental(){

}
