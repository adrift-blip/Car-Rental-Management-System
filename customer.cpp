#include "customer.h"
#include "user.h"
#include <fstream>
customer::customer(): User(){
    fullName = "N/A";
    liscenseNum = "N/A";
    loyaltyPoints = 0;
}
customer::customer(string userName, string password, string fullName, string liscenseNum, int loyaltyPoints): User(userName, password, 0){
    this->fullName = fullName;
    this->liscenseNum = liscenseNum;
    this->loyaltyPoints = loyaltyPoints;
}
customer::customer(customer& c): User(c.getUserName(), c.getUserPassword(), 0){
    this->fullName = c.fullName;
    this->liscenseNum = c.liscenseNum;
    this->loyaltyPoints = c.loyaltyPoints;
}
customer& customer::operator=(customer& other) {
    if(this != &other){
        this->setUserName(other.getUserName());
        this->setUserPassword(other.getUserPassword());
        this->setPrivilegeLevel(0);
        this->fullName = other.fullName;
        this->liscenseNum = other.liscenseNum;
        this->loyaltyPoints = other.loyaltyPoints;
    }
    return *this;
}
string customer::getFullName() const{
    return fullName;
}
string customer::getLiscenseNum() const{
    return liscenseNum;
}
int customer::getLoyaltyPoints() const{
    return loyaltyPoints;
}
void customer::setFullName(string fn){
    fullName = fn;
}
void customer::setLiscenseNum(string lic){
    liscenseNum = lic;
}
void customer::setLoyaltyPoints(int lp){
    loyaltyPoints = lp;
}
void customer::addPersonalDetails() const{
    ofstream outFile("../../data/users.txt", ios::app);
    if (!outFile) {
        throw invalid_argument("Unable to open file or maybe file does not exist");
    }
    outFile << fullName << "," << liscenseNum << "," << loyaltyPoints;
}
customer::~customer(){

}
