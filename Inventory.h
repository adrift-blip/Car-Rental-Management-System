/* I incuded these librarys so my code works.
   QFile lets me open and save the files on my hardrive.
   QTextStream makes it super easy for me to read the text line by line.
   QStringList helps me keep the splitted words in a nice array.
   QDebug is just so I can print stuff out on the black console screen. */
#ifndef INVENTORY_H
#define INVENTORY_H

#include <QFile>
#include <QTextStream>
#include <QStringList>

/* This class is for the car comapny.  */
class Manufacturer {
private:
    QString name;
public:
    Manufacturer(const QString& n = "Unknown");
    QString getName() const;
};








/* Here is my main base class for all the cars.*/
class Car {
protected:
    QString type;
    Manufacturer company;
    QString carName;
    QString colours;
    bool available;
    bool maintenanceRequired;
    double pricePerKm;
public:
    Car(QString t, QString comp, QString name,
        QString col, bool avail, bool maint, double price);

    QString getType()    const;
    QString getCompany() const;
    QString getName()    const;
    QString getColour()  const;
    bool    isAvailable()      const;
    bool    needsMaintenance() const;
    double  getPrice()         const;

    QString getBaseDataLeft()  const;
    QString getBaseDataRight() const;

    /* I made this pure virtual so every car type is forced to implement its own format. */
    virtual QString toSingleStr() const = 0;
    virtual ~Car() {}
};








/* This class inherrits from the Car class. */
class ElectricCar : public Car {
private:
    QString batteryPower;
public:
    ElectricCar(QString t, QString comp, QString name, QString battery, QString col, bool avail, bool maint, double price);

    /* I added getSpec so the GUI can grab the battery info directly without parsing. */
    QString getSpec()     const;
    QString toSingleStr() const override;
};







/* This class also inherrits from Car.  */
class PetrolCar : public Car {
private:
    QString engineCapacity;
public:
    PetrolCar(QString t, QString comp, QString name, QString engine, QString col, bool avail, bool maint, double price);

    /* Same as above but for engine capacity. */
    QString getSpec()     const;
    QString toSingleStr() const override;
};










/* This is my inventory class. I used basic arrays . */
class Inventory {
private:
    ElectricCar** evArray;
    PetrolCar**   petrolArray;
    int evCount,    petrolCount;
    int evCapacity, petrolCapacity;

    /* I store the file path here so saveToFile can write back to the same file without needing it passed in again. */
    QString loadedFilePath;

    void resizeEvArray();
    void resizePetrolArray();

public:
    Inventory();
    ~Inventory();

    void loadFromFile(const QString& fileName);

    /* Pass no argument to save back to the same file it was loaded from. */
    void saveToFile(const QString& fileName = "");
    void showFilteredCars(QString fType, QString fCompany);

    int          getEvCount()      const;
    int          getPetrolCount()  const;
    ElectricCar* getEvAt    (int i) const;
    PetrolCar*   getPetrolAt(int i) const;

    /* These two are for the GUI combo boxes so they can show only real values from the data. */
    QStringList getTypes()         const;
    QStringList getManufacturers() const;
};

#endif