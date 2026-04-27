#ifndef INVENTORY_H
#define INVENTORY_H

#include <string>
#include <iostream>

using namespace std;

class StringArray {
private:
    string* data;
    int count;
    int capacity;
    void resize();

public:
    StringArray();
    StringArray(const StringArray& other);
    StringArray& operator=(const StringArray& other);
    ~StringArray();

    void push(const string& val);
    bool contains(const string& val) const;
    void sort();
    int size() const;
    string operator[](int i) const;
};

class Car {
protected:
    string type;
    string company;
    string carName;
    int fuelType;
    string spec;
    string colours;
    bool available;
    bool maintenanceRequired;
    double pricePerKm;

public:
    Car(string t, string comp, string name, int fuel, string sp, string col, bool avail, bool maint, double price);
    virtual ~Car() {}

    string getType() const;
    string getCompany() const;
    string getName() const;
    int getFuelType() const;
    string getSpec() const;
    string getColour() const;
    bool isAvailable() const;
    bool needsMaintenance() const;
    double getPrice() const;

    void setAvailable(bool val);
    void setMaintenance(bool val);

    string getBaseDataLeft() const;
    string getBaseDataRight() const;
    virtual string toSingleStr() const;
};

class Sedan : public Car {
private:
    int trunkCapacity;
    bool hasSunroof;
public:
    Sedan(string t, string comp, string name, int fuel, string sp, string col, bool avail, bool maint, double price, int trunk, bool sun);
    int getTrunk() const;
    bool getSunroof() const;
};

class SUV : public Car {
private:
    bool is4WD;
    int groundClearanceMm;
public:
    SUV(string t, string comp, string name, int fuel, string sp, string col, bool avail, bool maint, double price, bool fwd, int gc);
    bool get4WD() const;
    int getGroundClearance() const;
};

class Hatchback : public Car {
private:
    bool hasRearWiper;
    bool foldableSeats;
public:
    Hatchback(string t, string comp, string name, int fuel, string sp, string col, bool avail, bool maint, double price, bool wiper, bool fold);
    bool getRearWiper() const;
    bool getFoldableSeats() const;
};

class Minivan : public Car {
private:
    int seatCount;
    bool slidingDoors;
public:
    Minivan(string t, string comp, string name, int fuel, string sp, string col, bool avail, bool maint, double price, int seats, bool sliding);
    int getSeats() const;
    bool getSlidingDoors() const;
};

class Inventory {
private:
    Car** carArray;
    int carCount;
    int carCapacity;
    string loadedFilePath;
    void resizeCarArray();

public:
    Inventory();
    ~Inventory();

    static StringArray splitLine(const string& line, char delim);
    static string toLower(const string& s);
    static string trim(const string& s);
    static string formatPrice(double price);

    void loadFromFile(const string& fileName);
    void saveToFile(const string& fileName = "");
    void addCar(Car* car);
    void removeCar(int index);
    int getCarCount() const;
    Car* getCarAt(int i) const;
    StringArray getTypes() const;
    StringArray getManufacturers() const;
};

#endif
