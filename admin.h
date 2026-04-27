#ifndef ADMIN_H
#define ADMIN_H
#include "user.h"
class admin: public User {
public:
    admin();
    admin(string userName, string userPassword, int privelegeLevel, string fullName, string empID);
    admin(admin &a);
    string getFullName();
    string getEmpID();
    void setFullName(string s);
    void setEmpID(string s);
    ~admin();
private:
    string empID;
    string fullName;
};

#endif // ADMIN_H
