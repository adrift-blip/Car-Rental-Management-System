#include "Inventory.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QDebug>
#include <iostream>
using namespace std;

/* I put this constructor here to set the name of the comapny. */
Manufacturer::Manufacturer(const QString& n) : name(n) {}

/* I use this function to get the name back out. */
QString Manufacturer::getName() const { return name; }


/* Sets up the base car data including the bools. */
Car::Car(QString t, QString comp, QString name, QString col, bool avail, bool maint, double price): type(t), company(comp), carName(name), colours(col), available(avail), maintenanceRequired(maint), pricePerKm(price) {}

/* Groups the first part of the single str row. */
QString Car::getBaseDataLeft() const {
    return QString("%1,%2,%3").arg(type, company.getName(), carName);
}

/* Groups the second part and turns bools into yes/no strings.
   I changed True/False to yes/no so the file saves in the same
   format it was loaded from — otherwise reload after save would break. */
QString Car::getBaseDataRight() const {

    return QString("%1,%2,%3,%4")
    .arg(colours) .arg(available ? "yes" : "no") .arg(maintenanceRequired ? "yes" : "no").arg(pricePerKm, 0, 'f', 2);
}

QString Car::getType()    const { return type; }
QString Car::getCompany() const { return company.getName(); }
QString Car::getName()    const { return carName; }
QString Car::getColour()  const { return colours; }
bool    Car::isAvailable()      const { return available; }
bool    Car::needsMaintenance() const { return maintenanceRequired; }
double  Car::getPrice()         const { return pricePerKm; }


/* Constructor for ev car. */
ElectricCar::ElectricCar(QString t, QString comp, QString name,QString battery, QString col, bool avail, bool maint, double price) : Car(t, comp, name, col, avail, maint, price), batteryPower(battery) {}

/* I added this so the GUI can read the spec directly instead of splitting toSingleStr. */
QString ElectricCar::getSpec() const { return batteryPower; }

/* EV CSV format. */
QString ElectricCar::toSingleStr() const {
    return QString("%1,EV,%2,%3").arg(getBaseDataLeft(), batteryPower, getBaseDataRight());
}


/* Constructor for petrol car. */
PetrolCar::PetrolCar(QString t, QString comp, QString name, QString engine, QString col, bool avail, bool maint, double price) : Car(t, comp, name, col, avail, maint, price), engineCapacity(engine) {}

/* Same as the EV one but returns engine capacity instead. */
QString PetrolCar::getSpec() const { return engineCapacity; }

/* Petrol CSV format. */
QString PetrolCar::toSingleStr() const {
    return QString("%1,Petrol,%2,%3").arg(getBaseDataLeft(), engineCapacity, getBaseDataRight());
}


/* Here I setup the dynamic arrays with a starting capacity of 5. */
Inventory::Inventory(): evCount(0), petrolCount(0), evCapacity(5), petrolCapacity(5)
{
    evArray     = new ElectricCar*[evCapacity];
    petrolArray = new PetrolCar*  [petrolCapacity];
}

/* My destructor deletes every object first and then deletes the dynamic array itself to be safe. */
Inventory::~Inventory() {

    for (int i = 0; i < evCount;     ++i) {
        delete evArray[i];
    }

    for (int i = 0; i < petrolCount; ++i) {
        delete petrolArray[i];
    }

    delete[] evArray;
    delete[] petrolArray;
}

/* This is my function to resize the ev array. I double the size and copy everything over to the new one. */
void Inventory::resizeEvArray() {

    evCapacity *= 2;
    ElectricCar** newArr = new ElectricCar*[evCapacity];
    for (int i = 0; i < evCount; ++i){
        newArr[i] = evArray[i];
    }

    delete[] evArray;
    evArray = newArr;
}

/* Same thing as above but for the petrol array. */
void Inventory::resizePetrolArray() {
    petrolCapacity *= 2;
    PetrolCar** newArr = new PetrolCar*[petrolCapacity];

    for (int i = 0; i < petrolCount; ++i) {
        newArr[i] = petrolArray[i];
    }
    delete[] petrolArray;
    petrolArray = newArr;
}

/* This reads the file and checks if I need more space before adding a new car. */
void Inventory::loadFromFile(const QString& fileName) {

    loadedFilePath = fileName;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString     line  = in.readLine().trimmed();
        QStringList parts = line.split(',');

        if (parts.size() < 9) continue;   // skip blank or malformed lines

        QString type          = parts[0].trimmed();
        QString company       = parts[1].trimmed();
        QString carName       = parts[2].trimmed();
        QString fuelType      = parts[3].trimmed().toLower();
        QString powerOrEngine = parts[4].trimmed();
        QString colours       = parts[5].trimmed();

        bool avail = (parts[6].trimmed().toLower() == "yes" ||parts[6].trimmed().toLower() == "true");

        bool maint = (parts[7].trimmed().toLower() == "yes" ||parts[7].trimmed().toLower() == "true");

        double price = parts[8].trimmed().toDouble();

        if (fuelType == "ev") {
            if (evCount == evCapacity) resizeEvArray();
            evArray[evCount++] = new ElectricCar(
                type, company, carName, powerOrEngine, colours, avail, maint, price);

        } else if (fuelType == "petrol") {
            if (petrolCount == petrolCapacity) resizePetrolArray();
            petrolArray[petrolCount++] = new PetrolCar(
                type, company, carName, powerOrEngine, colours, avail, maint, price);
        }
    }
    file.close();
}

/* Loops through the counts of each dynamic array and saves them to the file.
   If no filename is passed in it just writes back to the same file it loaded from. */
void Inventory::saveToFile(const QString& fileName) {

    const QString target = fileName.isEmpty() ? loadedFilePath : fileName;

    QFile file(target);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) return;

    QTextStream out(&file);
    for (int i = 0; i < evCount;     ++i){
        out << evArray[i]    ->toSingleStr() << "\n";
    }

    for (int i = 0; i < petrolCount; ++i){

        out << petrolArray[i]->toSingleStr() << "\n";
    }
    file.close();
}

int          Inventory::getEvCount()      const { return evCount; }
int          Inventory::getPetrolCount()  const { return petrolCount; }
ElectricCar* Inventory::getEvAt    (int i) const { return evArray[i]; }
PetrolCar*   Inventory::getPetrolAt(int i) const { return petrolArray[i]; }

/* This is my filtering logic. I loop through every car in my dynamic arrays and check if they match your words and my strict safety rules. */
void Inventory::showFilteredCars(QString fType, QString fCompany) {
    bool foundAny = false;
    QTextStream out(stdout);

    for (int i = 0; i < evCount; ++i) {

        bool typeMatch    = fType.isEmpty()    || (evArray[i]->getType()   .toLower() == fType.toLower());
        bool companyMatch = fCompany.isEmpty() || (evArray[i]->getCompany().toLower() == fCompany.toLower());

        if (typeMatch && companyMatch && evArray[i]->isAvailable() && !evArray[i]->needsMaintenance()) {
            out << "MATCH FOUND (EV): " << evArray[i]->toSingleStr() << Qt::endl;
            foundAny = true;
        }
    }

    for (int i = 0; i < petrolCount; ++i) {
        bool typeMatch    = fType.isEmpty()    || (petrolArray[i]->getType()   .toLower() == fType.toLower());
        bool companyMatch = fCompany.isEmpty() || (petrolArray[i]->getCompany().toLower() == fCompany.toLower());

        if (typeMatch && companyMatch && petrolArray[i]->isAvailable() && !petrolArray[i]->needsMaintenance()) {
            out << "MATCH FOUND (Petrol): " << petrolArray[i]->toSingleStr() << Qt::endl;
            foundAny = true;
        }
    }

    if (!foundAny) {
        out << "(No cars match those filters or safety rules)" << Qt::endl;
    }
}

/* Returns every unique car type found in the loaded data, sorted alphabetically. */
QStringList Inventory::getTypes() const {
    QStringList list;
    for (int i = 0; i < evCount; ++i){

        if (!list.contains(evArray[i]->getType())){
            list << evArray[i]->getType();
        }
    }
    for (int i = 0; i < petrolCount; ++i){
        if (!list.contains(petrolArray[i]->getType())){

            list << petrolArray[i]->getType();
        }
    }
    list.sort();
    return list;
}

/* Returns every unique manufacturer name found in the loaded data, sorted alphabetically. */
QStringList Inventory::getManufacturers() const {
    QStringList list;
    for (int i = 0; i < evCount; ++i){
        if (!list.contains(evArray[i]->getCompany())){
            list << evArray[i]->getCompany();
        }
    }
    for (int i = 0; i < petrolCount; ++i){
        if (!list.contains(petrolArray[i]->getCompany())){
            list << petrolArray[i]->getCompany();
        }
    }
    list.sort();
    return list;
}