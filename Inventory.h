#ifndef INVENTORY_H
#define INVENTORY_H

#include <string>
#include<iostream>
using namespace std;

/* ════════════════════════════════════════════════════════════
   StringArray
   A minimal resizable array of std::string that replaces
   a resizable dynamic array of std::string used throughout this project.
   ════════════════════════════════════════════════════════════ */
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
    int  size() const;
    string operator[](int i) const;
};


/* ════════════════════════════════════════════════════════════
   Manufacturer
   ════════════════════════════════════════════════════════════ */
class Manufacturer {
private:
    string name;
public:
    Manufacturer(const string& n = "Unknown");
    string getName() const;
};


/* ════════════════════════════════════════════════════════════
   Car  (abstract base)
   ════════════════════════════════════════════════════════════ */
class Car {
protected:
    string  type;
    Manufacturer company;
    string  carName;
    string  colours;
    bool         available;
    bool         maintenanceRequired;
    double       pricePerKm;

public:
    Car(string t, string comp, string name, string col, bool avail, bool maint, double price);

    string getType() const;
    string getCompany() const;
    string getName() const;
    string getColour() const;
    bool        isAvailable() const;
    bool        needsMaintenance() const;
    double      getPrice() const;

    string getBaseDataLeft() const;
    string getBaseDataRight() const;

    virtual string toSingleStr() const = 0;
    virtual ~Car() {}
};


/* ════════════════════════════════════════════════════════════
   ElectricCar
   ════════════════════════════════════════════════════════════ */
class ElectricCar : public Car {
private:
    string batteryPower;
public:
    ElectricCar(string t, string comp, string name, string battery, string col, bool avail, bool maint, double price);

    string getSpec()     const;
    string toSingleStr() const override;
};


/* ════════════════════════════════════════════════════════════
   PetrolCar
   ════════════════════════════════════════════════════════════ */
class PetrolCar : public Car {
private:
    string engineCapacity;
public:
    PetrolCar(string t, string comp, string name, string engine, string col, bool avail, bool maint, double price);

    string getSpec()     const;
    string toSingleStr() const override;
};


/* ════════════════════════════════════════════════════════════
   Inventory
   ════════════════════════════════════════════════════════════ */
class Inventory {
private:
    ElectricCar** evArray;
    PetrolCar** petrolArray;
    int evCount, petrolCount;
    int evCapacity, petrolCapacity;

    string loadedFilePath;

    void resizeEvArray();
    void resizePetrolArray();

    static StringArray splitLine(const string& line, char delim);
    static string toLower(const string& s);
    static string trim(const string& s);

public:
    Inventory();
    ~Inventory();

    void loadFromFile(const string& fileName);
    void saveToFile(const string& fileName = "");
    void showFilteredCars(string fType, string fCompany);

    int getEvCount() const;
    int getPetrolCount() const;
    ElectricCar* getEvAt (int i) const;
    PetrolCar* getPetrolAt(int i) const;

    StringArray getTypes() const;
    StringArray getManufacturers() const;
};

#endif