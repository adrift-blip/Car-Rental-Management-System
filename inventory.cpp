#include "inventory.h"
#include <fstream>
#include <string>
StringArray::StringArray() : data(new string[4]), count(0), capacity(4) {}
StringArray::StringArray(const StringArray& other) : count(other.count), capacity(other.capacity) {
    data = new string[capacity];
    for (int i = 0; i < count; ++i) data[i] = other.data[i];
}
StringArray& StringArray::operator=(const StringArray& other) {
    if (this != &other) {
        delete[] data;
        count = other.count;
        capacity = other.capacity;
        data = new string[capacity];
        for (int i = 0; i < count; ++i) data[i] = other.data[i];
    }
    return *this;
}
StringArray::~StringArray() { delete[] data; }
void StringArray::resize() {
    capacity *= 2;
    string* newData = new string[capacity];
    for (int i = 0; i < count; ++i) newData[i] = data[i];
    delete[] data;
    data = newData;
}
void StringArray::push(const string& val) {
    if (count == capacity) resize();
    data[count++] = val;
}
bool StringArray::contains(const string& val) const {
    for (int i = 0; i < count; ++i) if (data[i] == val) return true;
    return false;
}
void StringArray::sort() {
    for (int i = 0; i < count - 1; ++i) {
        for (int j = 0; j < count - i - 1; ++j) {
            if (data[j] > data[j + 1]) {
                string temp = data[j];
                data[j] = data[j + 1];
                data[j + 1] = temp;
            }
        }
    }
}
int StringArray::size() const { return count; }
string StringArray::operator[](int i) const { return data[i]; }

Car::Car(string id, string t, string comp, string name, int fuel, string sp, string col, bool avail, bool maint, double price)
    : cardID(id), type(t), company(comp), carName(name), fuelType(fuel), spec(sp), colours(col), available(avail), maintenanceRequired(maint), pricePerKm(price) {}
string Car::getType() const { return type; }
string Car::getCompany() const { return company; }
string Car::getName() const { return carName; }
int Car::getFuelType() const { return fuelType; }
string Car::getSpec() const { return spec; }
string Car::getColour() const { return colours; }
string Car::getCardId()const { return cardID; }
bool Car::isAvailable() const { return available; }
bool Car::needsMaintenance() const { return maintenanceRequired; }
double Car::getPrice() const { return pricePerKm; }
void Car::setAvailable(bool val) { available = val; }
void Car::setMaintenance(bool val) { maintenanceRequired = val; }
string Car::getBaseDataLeft() const { return type + "," + company + "," + carName; }
string Car::getBaseDataRight() const {
    string availStr = available ? "yes" : "no";
    string maintStr = maintenanceRequired ? "yes" : "no";
    return colours + "," + availStr + "," + maintStr + "," + Inventory::formatPrice(pricePerKm);
}
string Car::toSingleStr() const {
    string fStr = (fuelType == 1) ? "EV" : "Petrol";
    return getCardId() + "," + getBaseDataLeft() + "," + fStr + "," + spec + "," + getBaseDataRight();
}

Sedan::Sedan(string id, string t, string comp, string name, int fuel, string sp, string col, bool avail, bool maint, double price, int trunk, bool sun)
    : Car(id, t, comp, name, fuel, sp, col, avail, maint, price), trunkCapacity(trunk), hasSunroof(sun) {}
int Sedan::getTrunk() const { return trunkCapacity; }
bool Sedan::getSunroof() const { return hasSunroof; }

SUV::SUV(string id, string t, string comp, string name, int fuel, string sp, string col, bool avail, bool maint, double price, bool fwd, int gc)
    : Car(id, t, comp, name, fuel, sp, col, avail, maint, price), is4WD(fwd), groundClearanceMm(gc) {}
bool SUV::get4WD() const { return is4WD; }
int SUV::getGroundClearance() const { return groundClearanceMm; }

Hatchback::Hatchback(string id, string t, string comp, string name, int fuel, string sp, string col, bool avail, bool maint, double price, bool wiper, bool fold)
    : Car(id, t, comp, name, fuel, sp, col, avail, maint, price), hasRearWiper(wiper), foldableSeats(fold) {}
bool Hatchback::getRearWiper() const { return hasRearWiper; }
bool Hatchback::getFoldableSeats() const { return foldableSeats; }

Minivan::Minivan(string id, string t, string comp, string name, int fuel, string sp, string col, bool avail, bool maint, double price, int seats, bool sliding)
    : Car(id, t, comp, name, fuel, sp, col, avail, maint, price), seatCount(seats), slidingDoors(sliding) {}
int Minivan::getSeats() const { return seatCount; }
bool Minivan::getSlidingDoors() const { return slidingDoors; }

string Inventory::toLower(const string& s) {
    string res = s;
    for (int i = 0; i < res.length(); ++i) if (res[i] >= 'A' && res[i] <= 'Z') res[i] += 32;
    return res;
}
string Inventory::trim(const string& s) {
    if (s.empty()) return s;
    size_t first = s.find_first_not_of(" \t\r\n");
    if (first == string::npos) return "";
    size_t last = s.find_last_not_of(" \t\r\n");
    return s.substr(first, (last - first + 1));
}
string Inventory::formatPrice(double price) {
    int integral = (int)price;
    int fractional = (int)((price - integral) * 100 + 0.5);
    string fStr = to_string(fractional);
    if (fStr.length() < 2) fStr = "0" + fStr;
    return to_string(integral) + "." + fStr;
}
StringArray Inventory::splitLine(const string& line, char delim) {
    StringArray parts;
    string current;
    for (int i = 0; i < line.length(); ++i) {
        if (line[i] == delim) { parts.push(current); current = ""; }
        else current += line[i];
    }
    parts.push(current);
    return parts;
}
Inventory::Inventory() : carCount(0), carCapacity(10) { carArray = new Car*[carCapacity]; }
Inventory::~Inventory() {
    for (int i = 0; i < carCount; i++) delete carArray[i];
    delete[] carArray;
}
void Inventory::resizeCarArray() {
    carCapacity *= 2;
    Car** tmp = new Car*[carCapacity];
    for (int i = 0; i < carCount; i++) tmp[i] = carArray[i];
    delete[] carArray;
    carArray = tmp;
}
void Inventory::loadFromFile(const string& fileName) {
    loadedFilePath = fileName;
    ifstream file(fileName);
    string line;
    while (getline(file, line)) {
        StringArray p = splitLine(line, ',');
        if (p.size() < 10) continue;
        string carID = trim(p[0]);
        string type = trim(p[1]);
        string typeL = toLower(type);
        string comp = trim(p[2]), name = trim(p[3]), fuelStr = toLower(trim(p[4]));
        int fuelVal = (fuelStr == "ev") ? 1 : 0;
        string spec = trim(p[5]), col = trim(p[6]);
        bool avail = (toLower(trim(p[7])) == "yes"), maint = (toLower(trim(p[8])) == "yes");
        double pr = stod(trim(p[9]));
        if (carCount == carCapacity) resizeCarArray();
        if (typeL == "suv") carArray[carCount++] = new SUV(carID, type, comp, name, fuelVal, spec, col, avail, maint, pr, true, 210);
        else if (typeL == "sedan") carArray[carCount++] = new Sedan(carID, type, comp, name, fuelVal, spec, col, avail, maint, pr, 500, true);
        else if (typeL == "minivan") carArray[carCount++] = new Minivan(carID, type, comp, name, fuelVal, spec, col, avail, maint, pr, 7, true);
        else carArray[carCount++] = new Hatchback(carID, type, comp, name, fuelVal, spec, col, avail, maint, pr, true, true);
    }
}
void Inventory::saveToFile(const string& fileName) {
    ofstream file(fileName.empty() ? loadedFilePath : fileName);
    for (int i = 0; i < carCount; i++) file << carArray[i]->toSingleStr() << endl;
}
int Inventory::getCarCount() const { return carCount; }
Car* Inventory::getCarAt(int i) const { return carArray[i]; }
StringArray Inventory::getTypes() const {
    StringArray res;
    for (int i = 0; i < carCount; i++) if (!res.contains(carArray[i]->getType())) res.push(carArray[i]->getType());
    res.sort();
    return res;
}
StringArray Inventory::getManufacturers() const {
    StringArray res;
    for (int i = 0; i < carCount; i++) if (!res.contains(carArray[i]->getCompany())) res.push(carArray[i]->getCompany());
    res.sort();
    return res;
}


void Inventory::addCar(Car* car) {
    if (carCount == carCapacity) {
        resizeCarArray();
    }
    carArray[carCount] = car;
    carCount = carCount + 1;
}

void Inventory::removeCar(int index) {
    if (index >= 0) {
        if (index < carCount) {
            delete carArray[index];
            for (int i = index; i < carCount - 1; ++i) {
                carArray[i] = carArray[i + 1];
            }
            carCount = carCount - 1;
        }
    }
}
Car* Inventory::getCarFromID(string CarID){
    Car* car;
    for(int i = 0; i < carCount; i++){
        car = getCarAt(i);
        if(car->getCardId() == CarID){
            return car;
        }
    }
    return nullptr;
}
