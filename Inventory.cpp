#include "Inventory.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include <iostream>
using namespace std;


/* ════════════════════════════════════════════════════════════
   StringArray
   ════════════════════════════════════════════════════════════ */

StringArray::StringArray() : data(new string[4]), count(0), capacity(4) {}

/* Deep-copy constructor. */
StringArray::StringArray(const StringArray& other) : data(new string[other.capacity]), count(other.count), capacity(other.capacity)
{
    for (int i = 0; i < count; ++i)
        data[i] = other.data[i];
}

/* Copy-assignment operator. */
StringArray& StringArray::operator=(const StringArray& other)
{
    if (this == &other) return *this;
    delete[] data;
    capacity = other.capacity;
    count = other.count;
    data = new string[capacity];
    for (int i = 0; i < count; ++i)
        data[i] = other.data[i];
    return *this;
}

StringArray::~StringArray() { delete[] data; }

/* Doubles the internal buffer when full. */
void StringArray::resize()
{
    capacity *= 2;
    string* newData = new string[capacity];
    for (int i = 0; i < count; ++i)
        newData[i] = data[i];
    delete[] data;
    data = newData;
}

void StringArray::push(const string& val)
{
    if (count == capacity) resize();
    data[count++] = val;
}

bool StringArray::contains(const string& val) const
{
    for (int i = 0; i < count; ++i)
        if (data[i] == val) return true;
    return false;
}

/* Simple bubble sort — string counts are small so this is fine. */
void StringArray::sort()
{
    for (int i = 0; i < count - 1; ++i)
        for (int j = 0; j < count - 1 - i; ++j)
            if (data[j] > data[j + 1])
                std::swap(data[j], data[j + 1]);
}

int StringArray::size() const { return count; }
string StringArray::operator[](int i) const
{
    if (i < 0 || i >= count) return "";
    return data[i];
}


/* ════════════════════════════════════════════════════════════
   Private static helpers
   ════════════════════════════════════════════════════════════ */

/* Splits line by delim and returns each token in a StringArray. */
StringArray Inventory::splitLine(const string& line, char delim)
{
    StringArray result;
    string current;
    for (char ch : line) {
        if (ch == delim) {
            result.push(current);
            current.clear();
        } else {
            current += ch;
        }
    }
    result.push(current);   /* push the final token */
    return result;
}

/* Returns a lowercase copy of s. */
string Inventory::toLower(const string& s)
{
    string r = s;
    for (char& c : r) c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
    return r;
}

/* Removes leading and trailing whitespace. */
string Inventory::trim(const string& s)
{
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}


/* ════════════════════════════════════════════════════════════
   Manufacturer
   ════════════════════════════════════════════════════════════ */

Manufacturer::Manufacturer(const string& n) : name(n) {}
string Manufacturer::getName() const { return name; }


/* ════════════════════════════════════════════════════════════
   Car (base)
   ════════════════════════════════════════════════════════════ */

Car::Car(string t, string comp, string name, string col, bool avail, bool maint, double price) : type(t), company(comp), carName(name), colours(col), available(avail), maintenanceRequired(maint), pricePerKm(price) {}

string Car::getBaseDataLeft() const
{
    return type + "," + company.getName() + "," + carName;
}

string Car::getBaseDataRight() const
{
    ostringstream oss;
    oss << colours << "," << (available ? "yes" : "no") << "," << (maintenanceRequired ? "yes" : "no") << "," << fixed << setprecision(2) << pricePerKm;
    return oss.str();
}

string Car::getType() const { return type; }
string Car::getCompany() const { return company.getName(); }
string Car::getName() const { return carName; }
string Car::getColour() const { return colours; }
bool Car::isAvailable() const { return available; }
bool Car::needsMaintenance() const { return maintenanceRequired; }
double Car::getPrice() const { return pricePerKm; }


/* ════════════════════════════════════════════════════════════
   ElectricCar
   ════════════════════════════════════════════════════════════ */

ElectricCar::ElectricCar(string t, string comp, string name, string battery, string col, bool avail, bool maint, double price) : Car(t, comp, name, col, avail, maint, price), batteryPower(battery) {}

string ElectricCar::getSpec() const { return batteryPower; }

string ElectricCar::toSingleStr() const
{
    return getBaseDataLeft() + ",EV," + batteryPower + "," + getBaseDataRight();
}


/* ════════════════════════════════════════════════════════════
   PetrolCar
   ════════════════════════════════════════════════════════════ */

PetrolCar::PetrolCar(string t, string comp, string name, string engine, string col, bool avail, bool maint, double price) : Car(t, comp, name, col, avail, maint, price), engineCapacity(engine) {}

string PetrolCar::getSpec() const { return engineCapacity; }

string PetrolCar::toSingleStr() const
{
    return getBaseDataLeft() + ",Petrol," + engineCapacity + "," + getBaseDataRight();
}


/* ════════════════════════════════════════════════════════════
   Inventory — constructor / destructor
   ════════════════════════════════════════════════════════════ */

Inventory::Inventory() : evCount(0), petrolCount(0), evCapacity(5), petrolCapacity(5)
{
    evArray     = new ElectricCar*[evCapacity];
    petrolArray = new PetrolCar*  [petrolCapacity];
}

Inventory::~Inventory()
{
    for (int i = 0; i < evCount; ++i) delete evArray[i];
    for (int i = 0; i < petrolCount; ++i) delete petrolArray[i];
    delete[] evArray;
    delete[] petrolArray;
}


/* ════════════════════════════════════════════════════════════
   Resize helpers
   ════════════════════════════════════════════════════════════ */

void Inventory::resizeEvArray()
{
    evCapacity *= 2;
    ElectricCar** newArr = new ElectricCar*[evCapacity];
    for (int i = 0; i < evCount; ++i) newArr[i] = evArray[i];
    delete[] evArray;
    evArray = newArr;
}

void Inventory::resizePetrolArray()
{
    petrolCapacity *= 2;
    PetrolCar** newArr = new PetrolCar*[petrolCapacity];
    for (int i = 0; i < petrolCount; ++i) newArr[i] = petrolArray[i];
    delete[] petrolArray;
    petrolArray = newArr;
}


/* ════════════════════════════════════════════════════════════
   loadFromFile
   ════════════════════════════════════════════════════════════ */

void Inventory::loadFromFile(const string& fileName)
{
    loadedFilePath = fileName;

    ifstream file(fileName);
    if (!file.is_open()) return;

    string line;
    while (getline(file, line)) {
        StringArray parts = splitLine(line, ',');
        if (parts.size() < 9) continue;   /* skip blank or malformed lines */

        string type          = trim(parts[0]);
        string company       = trim(parts[1]);
        string carName       = trim(parts[2]);
        string fuelType      = toLower(trim(parts[3]));
        string powerOrEngine = trim(parts[4]);
        string colours       = trim(parts[5]);

        string availStr = toLower(trim(parts[6]));
        string maintStr = toLower(trim(parts[7]));

        bool avail = (availStr == "yes" || availStr == "true");
        bool maint = (maintStr == "yes" || maintStr == "true");

        double price = stod(trim(parts[8]));

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


/* ════════════════════════════════════════════════════════════
   saveToFile
   ════════════════════════════════════════════════════════════ */

void Inventory::saveToFile(const string& fileName)
{
    const string target = fileName.empty() ? loadedFilePath : fileName;

    ofstream file(target);
    if (!file.is_open()) return;

    for (int i = 0; i < evCount;     ++i)
        file << evArray[i]->toSingleStr()     << "\n";
    for (int i = 0; i < petrolCount; ++i)
        file << petrolArray[i]->toSingleStr() << "\n";

    file.close();
}


/* ════════════════════════════════════════════════════════════
   Accessors
   ════════════════════════════════════════════════════════════ */

int Inventory::getEvCount() const { return evCount; }
int Inventory::getPetrolCount() const { return petrolCount; }
ElectricCar* Inventory::getEvAt (int i) const { return evArray[i]; }
PetrolCar*   Inventory::getPetrolAt(int i) const { return petrolArray[i]; }


/* ════════════════════════════════════════════════════════════
   showFilteredCars — console output
   ════════════════════════════════════════════════════════════ */

void Inventory::showFilteredCars(string fType, string fCompany)
{
    bool foundAny = false;

    for (int i = 0; i < evCount; ++i) {
        bool typeMatch    = fType.empty()    || toLower(evArray[i]->getType())    == toLower(fType);
        bool companyMatch = fCompany.empty() || toLower(evArray[i]->getCompany()) == toLower(fCompany);

        if (typeMatch && companyMatch &&
            evArray[i]->isAvailable() && !evArray[i]->needsMaintenance())
        {
            cout << "MATCH FOUND (EV): " << evArray[i]->toSingleStr() << "\n";
            foundAny = true;
        }
    }

    for (int i = 0; i < petrolCount; ++i) {
        bool typeMatch    = fType.empty()    || toLower(petrolArray[i]->getType())    == toLower(fType);
        bool companyMatch = fCompany.empty() || toLower(petrolArray[i]->getCompany()) == toLower(fCompany);

        if (typeMatch && companyMatch &&
            petrolArray[i]->isAvailable() && !petrolArray[i]->needsMaintenance())
        {
            cout << "MATCH FOUND (Petrol): " << petrolArray[i]->toSingleStr() << "\n";
            foundAny = true;
        }
    }

    if (!foundAny)
        cout << "(No cars match those filters or safety rules)\n";
}


/* ════════════════════════════════════════════════════════════
   getTypes / getManufacturers
   ════════════════════════════════════════════════════════════ */

StringArray Inventory::getTypes() const
{
    StringArray list;
    for (int i = 0; i < evCount; ++i)
        if (!list.contains(evArray[i]->getType()))
            list.push(evArray[i]->getType());
    for (int i = 0; i < petrolCount; ++i)
        if (!list.contains(petrolArray[i]->getType()))
            list.push(petrolArray[i]->getType());
    list.sort();
    return list;
}

StringArray Inventory::getManufacturers() const
{
    StringArray list;
    for (int i = 0; i < evCount; ++i)
        if (!list.contains(evArray[i]->getCompany()))
            list.push(evArray[i]->getCompany());
    for (int i = 0; i < petrolCount; ++i)
        if (!list.contains(petrolArray[i]->getCompany()))
            list.push(petrolArray[i]->getCompany());
    list.sort();
    return list;
}