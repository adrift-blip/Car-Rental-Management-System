#ifndef RENTALHISTORY_H
#define RENTALHISTORY_H
#include "rental.h"
class rentalHistory
{
public:
    rentalHistory();
    rentalHistory(string filename);
    rentalHistory(string filename, string userName);
    rentalHistory(const rentalHistory &other);
    rentalHistory& operator=(const rentalHistory &other);
    rental getRentalAt(int index);
    int getNoOfRentals();
    void appendNewRental(const rental& r);
    void setStatusAt(int index, string status);
    ~rentalHistory();
protected:
    rental* array;
    int noOfRentals;
    friend class customer;
};

#endif // RENTALHISTORY_H
