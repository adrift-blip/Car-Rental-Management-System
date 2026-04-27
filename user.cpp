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
void User::signUp() const{
    ifstream inFile("../../data/users.txt");
    string line;
    if (!inFile) {
        return;
    }
    while (getline(inFile, line)) {
        stringstream sample(line);
        string toConvert, currentUserName, currentUserPassword;
        getline(sample, currentUserName, ',');
        //An equal operator is not overloaded and used here as when it was used it crashed the program
        if (currentUserName == this->userName) {
            throw invalid_argument("This user already exists! Please return to sign in");
        }
    }
    inFile.close();
    ofstream outFile("../../data/users.txt", ios::app);
    if (!outFile) {
        throw invalid_argument("Unable to open file or maybe file does not exist");
    }
    outFile << endl << userName << "," << userPassword << "," << privilegeLevel << "," << flush;
    outFile.close();
}
