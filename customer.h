#ifndef CUSTOMER_H
#define CUSTOMER_H
#include "user.h"
#include "rentalhistory.h"
class customer: public User
{
private:
    string fullName;
    string liscenseNum;
    int loyaltyPoints;
    rentalHistory rh;
public:
    customer();
    customer(string userName, string password, string fullName, string liscenseNum, int loyaltyPoints = 0);
    customer(const customer& c);
    customer& operator=(const customer& other);
    string getFullName() const;
    string getLiscenseNum() const;
    int getLoyaltyPoints() const;
    void setFullName(string fn);
    void setLiscenseNum(string lic);
    void setLoyaltyPoints(int lp);
    void addPersonalDetails() const;
    rentalHistory& getRentalHistory();
    void updateLoyaltyPoints(int pointsToAdd);
    ~customer();
};

#endif // CUSTOMER_H
