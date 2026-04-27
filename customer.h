#ifndef CUSTOMER_H
#define CUSTOMER_H
#include "user.h"
class customer: public User
{
private:
    string fullName;
    string liscenseNum;
    int loyaltyPoints;
public:
    customer();
    customer(string userName, string password, string fullName, string liscenseNum, int loyaltyPoints = 0);
    customer(customer& c);
    customer& operator=(customer& other);
    string getFullName() const;
    string getLiscenseNum() const;
    int getLoyaltyPoints() const;
    void setFullName(string fn);
    void setLiscenseNum(string lic);
    void setLoyaltyPoints(int lp);
    void addPersonalDetails() const;
    ~customer();
};

#endif // CUSTOMER_H
