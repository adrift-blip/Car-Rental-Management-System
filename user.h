#ifndef USER_H
#define USER_H

#include <string>
using namespace std;

class User {
private:
    string userName;
    string userPassword;
    int privilegeLevel;
public:
    User();
    User(string inputName, string inputPassword, int inputPrivilegeLevel);
    User(const User &user);
    ~User();
    void setUserName(string userName);
    void setUserPassword(string userPassword);
    void setPrivilegeLevel(int privilegeLevel);
    string getUserName()const;
    string getUserPassword()const;
    int getPrivilegeLevel()const;
    bool loginValid() const;
    void signUp() const;
    void removeUser() const;
    void addAdmin() const;
    User& operator=(User& other){
        if(this != &other){
            userName = other.userName;
            userPassword = other.userPassword;
            privilegeLevel = other.privilegeLevel;
        }
        return *this;
    }
    friend istream &operator>>(istream &in, User &user);
};
istream &operator>>(istream &in, User &user);


#endif // USER_H
