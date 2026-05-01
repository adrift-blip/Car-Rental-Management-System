//
// Created by hamda on 3/17/2026.
//

#include "user.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include<stdexcept>
using namespace std;
User::User() {
    userName = "N/A";
    userPassword = "N/A";
    privilegeLevel = -1;
}
User::User(string inputName, string inputPassword, int inputPrivilegeLevel) {
    userName = inputName;
    userPassword = inputPassword;
    privilegeLevel = inputPrivilegeLevel;
}
User::User(const User &user) {
    this->userName = user.userName;
    this->userPassword = user.userPassword;
    this->privilegeLevel = user.privilegeLevel;
}
User::~User() {
    cout << "User Destructor" << endl;
}
void User::setUserName(string inputName) {
    this->userName = inputName;
}
void User::setUserPassword(string inputPassword) {
    this->userPassword = inputPassword;
}
void User::setPrivilegeLevel(int inputPrivilegeLevel) {
    this->privilegeLevel = inputPrivilegeLevel;
}
int User::getPrivilegeLevel()const {
    return privilegeLevel;
}
string User::getUserName()const {
    return userName;
}
string User::getUserPassword()const {
    return userPassword;
}
istream &operator>>(istream &in, User &user) {
    cout << "Enter user name: ";
    in.ignore();
    getline(in, user.userName);
    cout << "Enter user password: ";
    getline(in, user.userPassword);
    return in;
}
bool User::loginValid() const{
    ifstream inFile("../../data/users.txt");
    string line;
    if (!inFile) {
        cout << "FILE NOT FOUND" << endl;
        return false;
    }
    while (getline(inFile, line)) {
        stringstream sample(line);
        string toConvert, currentUserName, currentUserPassword;
        getline(sample, currentUserName, ',');
        getline(sample, currentUserPassword, ',');
        getline(sample, toConvert, ',');
        int currentPrivilegeLevel = stoi(toConvert);
        //An equal operator is not overloaded and used here as when it was used it crashed the program
        if (currentUserName == this->userName && currentUserPassword == this->userPassword && currentPrivilegeLevel == this->privilegeLevel) {
            return true;
        }
    }
    inFile.close();
    return false;
}
bool User::signUp() const{
    ifstream inFile("../../data/users.txt");
    string line;
    if (!inFile) {
        return false;
    }
    while (getline(inFile, line)) {
        stringstream sample(line);
        string toConvert, currentUserName, currentUserPassword;
        getline(sample, currentUserName, ',');
        //An equal operator is not overloaded and used here as when it was used it crashed the program
        if (currentUserName == this->userName) {
            inFile.close();
            return false;
        }
    }
    inFile.close();
    return true;
}
bool User::isStrongPassword(const string& password) {
    if (password.length() < 8) return false;

    bool hasUpper = false, hasLower = false, hasDigit = false, hasSpecial = false;

    for (char c : password) {
        if (isupper(c)) hasUpper = true;
        if (islower(c)) hasLower = true;
        if (isdigit(c)) hasDigit = true;
        if (ispunct(c)) hasSpecial = true;
    }
    return hasUpper && hasLower && hasDigit && hasSpecial;
}