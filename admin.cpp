#include "admin.h"
#include "user.h"
admin::admin(): User("N/A", "N/A", 1) {
    string fullName = "N/A";
    string empID = "N/A";
}
admin::admin(string userName, string userPassword, int privelegeLevel, string fullName, string empID):User(userName, userPassword, privelegeLevel){
    this->fullName = fullName;
    this->empID = empID;
}
admin::admin(admin &a):User(a.getUserName(), a.getUserPassword(), 1){
    this->fullName = a.getFullName();
    this->empID = a.getEmpID();
}
string admin::getFullName(){
    return fullName;
}
string admin::getEmpID(){
    return empID;
}
void admin::setFullName(string s){
    this->fullName = s;
}
void admin::setEmpID(string s){
    this->empID = s;
}
admin::~admin(){

}
