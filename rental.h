#ifndef RENTAL_H
#define RENTAL_H
#include <string>
using namespace std;
class rental
{
public:
    rental();
    rental(string id, string name, string sDate, string eDate, string carID, double tc, string status, bool driver, bool insurance, bool delivery, double discountRate);
    rental(const rental& other);
    rental& operator=(const rental &other);
    string getRentalID() const;
    string getNameOfBooker() const;
    string getStartDate() const;
    string getEndDate() const;
    string getCardID() const;
    string getStatus() const;
    void setStatus(string s);
    double getRentalPrice() const;
    bool getHasDriver() const;
    bool getHasInsurance() const;
    bool getHasDelivery() const;
    double getDiscountRate() const;
    void setHasDriver(bool val);
    void setHasInsurance(bool val);
    void setHasDelivery(bool val);
    void setDiscountRate(double rate);
    ~rental();
private:
    string rentalID;
    string userNameOfBooker;
    string startDate;
    string endDate;
    string carID;
    string status;
    double totalCost;
    bool hasDriver;
    bool hasInsurance;
    bool hasDelivery;
    double discountRate;
};

#endif
