#include "customer.h"
#include "user.h"
#include <fstream>
#include <sstream>
customer::customer(): User(), rh(){
    fullName = "N/A";
    liscenseNum = "N/A";
    loyaltyPoints = 0;
}
customer::customer(string userName, string password, string fullName, string liscenseNum, int loyaltyPoints): User(userName, password, 0), rh("../../data/rental.txt", userName){
    this->fullName = fullName;
    this->liscenseNum = liscenseNum;
    this->loyaltyPoints = loyaltyPoints;
}
customer::customer(const customer& c): User(c.getUserName(), c.getUserPassword(), 0), rh(c.rh){
    this->fullName = c.fullName;
    this->liscenseNum = c.liscenseNum;
    this->loyaltyPoints = c.loyaltyPoints;
}
customer& customer::operator=(const customer& other) {
    if(this != &other){
        this->setUserName(other.getUserName());
        this->setUserPassword(other.getUserPassword());
        this->setPrivilegeLevel(0);
        this->fullName = other.fullName;
        this->liscenseNum = other.liscenseNum;
        this->loyaltyPoints = other.loyaltyPoints;
        rh = other.rh;
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
    outFile << endl << getUserName() << "," << getUserPassword() << ",0," << fullName << "," << liscenseNum << "," << loyaltyPoints;
}
rentalHistory& customer::getRentalHistory(){
    return rh;
}
void customer::updateLoyaltyPoints(int pointsToAdd) {
    loyaltyPoints += pointsToAdd;
    ifstream inFile("../../data/users.txt");
    ofstream tempFile("../../data/users_temp.txt");
    if (!inFile || !tempFile) return;

    string line;
    while (getline(inFile, line)) {
        stringstream ss(line);
        string uname;
        getline(ss, uname, ',');
        if (uname == getUserName()) {
            string pass, priv, fname, lic, pts;
            getline(ss, pass, ',');
            getline(ss, priv, ',');
            getline(ss, fname, ',');
            getline(ss, lic, ',');
            tempFile << uname << "," << pass << "," << priv << "," << fname << "," << lic << "," << loyaltyPoints;
        } else {
            tempFile << line;
        }
        tempFile << "\n";
    }
    inFile.close();
    tempFile.close();
    remove("../../data/users.txt");
    rename("../../data/users_temp.txt", "../../data/users.txt");
}
customer::~customer(){

}
