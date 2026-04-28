#include "rentalhistory.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;
rentalHistory::rentalHistory() {
    array = nullptr;
    noOfRentals = 0;
}
rentalHistory::rentalHistory(string filename, string userName){
    noOfRentals = 0;
    ifstream inFile(filename);
    if (!inFile) {
        cout << "FILE NOT FOUND" << endl;
        exit(1);
    }
    string line;
    while (getline(inFile, line)) {
        stringstream sample(line);
        string rentalID, currentUserName;
        getline(sample, rentalID, ',');
        getline(sample, currentUserName, ',');
        if (currentUserName == userName) {
            noOfRentals++;
        }
    }
    inFile.clear();
    inFile.seekg(0);
    array = new rental[noOfRentals];
    int i = 0;
    while (getline(inFile, line)) {
        stringstream sample(line);
        string rentalID, currentUserName, sDate, eDate, carID, state, tcString;
        double tc;
        getline(sample, rentalID, ',');
        getline(sample, currentUserName, ',');
        getline(sample, sDate, ',');
        getline(sample, eDate, ',');
        getline(sample, carID, ',');
        getline(sample, tcString, ',');
        getline(sample, state);
        tc = stof(tcString);
        if (currentUserName == userName) {
            *(array + i) = rental (rentalID, currentUserName, sDate, eDate, carID, tc, state);
            i++;
        }
    }
    inFile.close();
}
rentalHistory::rentalHistory(const rentalHistory &other){
    noOfRentals = other.noOfRentals;
    array = new rental[noOfRentals];
    for(int i = 0; i < noOfRentals; i++){
        *(array + i) = *(other.array + i);
    }
}
rentalHistory& rentalHistory::operator=(const rentalHistory &other){
    if(this != &other){
        delete[]this->array;
        array = nullptr;
        noOfRentals = other.noOfRentals;
        array = new rental[noOfRentals];
        for(int i = 0; i < noOfRentals; i++){
            *(array + i) = *(other.array + i);
        }
    }
    return *this;
}
rentalHistory::~rentalHistory(){
    delete[]array;
    array = nullptr;
}
rental rentalHistory:: getRentalAt(int index){
    if(index < 0 || index > noOfRentals)
        throw invalid_argument("The given car does not exist");
    else
        return *(array + index);
}
int rentalHistory::getNoOfRentals(){
    return noOfRentals;
}
void rentalHistory::appendNewRental(const rental& r){
    rentalHistory temp(*this);
    delete[]array;
    noOfRentals++;
    array = new rental[noOfRentals];
    for(int k = 0; k < temp.noOfRentals; k++){
        *(array + k) = *(temp.array + k);
    }
    *(array + noOfRentals - 1) = rental(r);
    ofstream inFile("../../data/rental.txt", ios::app);
    if(!inFile){
        exit(1);
    }
    inFile << endl << r.getRentalID() << "," << r.getNameOfBooker() << "," << r.getStartDate() << "," << r.getEndDate() << "," << r.getCardID() << "," << r.getRentalPrice() << "," << r.getStatus();
    inFile.close();
}
