#include "userdashboard.h"
#include "ui_userdashboard.h"
#include <ctime>
#include <QTimer>
#include "inventory.h"
#include "rental.h"
#include "rentalhistory.h"
#include "signinform.h"
#include <string.h>
#include <QString>
#include <QColor>
#include <QMessageBox>
#include<QHeaderView>
using namespace std;
userDashboard::userDashboard(customer c, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::userDashboard), c(c)
{
    i = new Inventory();
    this->setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
    connect(ui->overviewBtn, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentWidget(ui->overviewPage);
    });
    connect(ui->browseBtn, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentWidget(ui->browsePage);
        QTimer::singleShot(0, this, [this](){
        applyFilter();
        });
    });
    connect(ui->newBtn, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentWidget(ui->newBookingPage);
    });
    connect(ui->bookingsBtn, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentWidget(ui->myBookingPage);
        loadFromHistory();
    });
    connect(ui->invoiceBtn, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentWidget(ui->overviewPage);
    });
    connect(ui->profileBtn, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentWidget(ui->profilePage);
    });
    string fullName = this->c.getFullName(); string initials;
    char *ptr = &fullName[0];
    for(int i = 0; ptr[i] != '\0'; i++){
        if(i == 0){
            initials += ptr[i];
        }
        else if(ptr[i] == ' ' && ptr[i + 1] != '\0'){
            if((ptr[i + 1] >= 'A' && ptr[i + 1] <= 'Z') || (ptr[i + 1] >= 'a' && ptr[i + 1] <= 'z')){
                initials += ptr[i + 1];
            }
        }
    }
    QString qInitials = QString::fromStdString(initials).toUpper();
    ui->avatarDisplay->setText(qInitials);
    ui->avatarLabel->setText(qInitials);
    ui->customerName->setText(QString::fromStdString(fullName));
    ui->fullNameLabel->setText(QString::fromStdString(fullName));
    ui->password->setText(QString::fromStdString(c.getUserPassword()));
    ui->liscenseNumField->setText(QString::fromStdString(c.getLiscenseNum()));
    ui->whatPoints->setText(QString::number(c.getLoyaltyPoints()));
    int loyaltyPoints = this->c.getLoyaltyPoints();
    if(loyaltyPoints >= 750){
        ui->membershipTier->setText("Platinum");
        ui->tierLabel->setText("Platinum");
        ui->whatTier->setText("Platinum");
    }
    else if(loyaltyPoints >= 400 && loyaltyPoints < 750){
        ui->membershipTier->setText("Gold");
        ui->tierLabel->setText("Gold");
        ui->whatTier->setText("Gold");
    }
    else if(loyaltyPoints >= 100 && loyaltyPoints < 400){
        ui->membershipTier->setText("Silver");
        ui->tierLabel->setText("Silver");
        ui->whatTier->setText("Silver");
    }
    else{
        ui->membershipTier->setText("Bronze");
        ui->tierLabel->setText("Bronze");
        ui->whatTier->setText("Bronze");
    }
    string greeting = "Good Day"; greeting += " "; greeting += fullName;
    time_t currentTime = time(0);
    tm* ltm = localtime(&currentTime);
    string date = to_string(ltm->tm_mday) + "/" + to_string(ltm->tm_mon + 1) + "/" + to_string(ltm->tm_year + 1900);
    i->loadFromFile("../../data/inventory.txt");
    initializeFleet();
    createHistoryTable();
    initializeBookingPage();
}
void userDashboard::on_logoutBtn_clicked(){
    QApplication::quit();
}
void userDashboard::initializeFleet(){
    ui->typeComboBox->clear();
    ui->typeComboBox->addItem("All Types");
    StringArray typeL = i->getTypes();
    for(int i = 0; i < typeL.size(); i++){
        ui->typeComboBox->addItem(QString::fromStdString(typeL[i]));
    }
    ui->manufacturerComboBox->clear();
    ui->manufacturerComboBox->addItem("All Manufacturers");
    StringArray typeB = i->getManufacturers();
    for(int i = 0; i < typeB.size(); i++){
        ui->manufacturerComboBox->addItem(QString::fromStdString(typeB[i]));
    }
    ui->fuelComboBox->clear();
    ui->fuelComboBox->addItem("All Fuel");
    ui->fuelComboBox->addItem("Petrol");
    ui->fuelComboBox->addItem("Electric");
    ui->fleetTable->setColumnCount(6);
    QStringList headers;
    headers << "Name" << "Type" << "Brand" << "Fuel" << "Spec" << "Price/km";
    ui->fleetTable->setHorizontalHeaderLabels(headers);
    ui->fleetTable->setColumnWidth(0, 160);
    ui->fleetTable->setColumnWidth(1, 100);
    ui->fleetTable->setColumnWidth(2, 120);
    ui->fleetTable->setColumnWidth(3, 80);
    ui->fleetTable->setColumnWidth(4, 140);
    ui->fleetTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->fleetTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->fleetTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->fleetTable->setFocusPolicy(Qt::NoFocus);
    ui->fleetTable->verticalHeader()->setVisible(false);
    ui->fleetTable->setShowGrid(false);
    ui->fleetTable->setAlternatingRowColors(true);
    ui->fleetTable->horizontalHeader()->setStretchLastSection(true);
    ui->fleetTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    ui->fleetTable->verticalHeader()->setDefaultSectionSize(48);
    connect(ui->fleetTable, &QTableWidget::cellClicked, this, &userDashboard::onFleetRowClicked);
    connect(ui->applyBtn, &QPushButton::clicked, this, &userDashboard::applyFilter);
    connect(ui->resetButton, &QPushButton::clicked, this, [this](){
        ui->typeComboBox->setCurrentIndex(0);
        ui->manufacturerComboBox->setCurrentIndex(0);
        ui->fuelComboBox->setCurrentIndex(0);
        ui->availabilityCheckBox->setChecked(false);
        applyFilter();
    });
}
void userDashboard::createHistoryTable(){
    ui->rentalHistoryTable->setColumnCount(7);
    QStringList headers;
    headers << "RentalID" << "Customer Name" << "Car Name" << "Start Date" << "End Date" << "Total Cost" << "Status";
    ui->rentalHistoryTable->setHorizontalHeaderLabels(headers);
    ui->rentalHistoryTable->setColumnWidth(0, 100);
    ui->rentalHistoryTable->setColumnWidth(1, 160);
    ui->rentalHistoryTable->setColumnWidth(2, 180);
    ui->rentalHistoryTable->setColumnWidth(3, 110);
    ui->rentalHistoryTable->setColumnWidth(4, 110);
    ui->rentalHistoryTable->setColumnWidth(5, 110);
    ui->rentalHistoryTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->rentalHistoryTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->rentalHistoryTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->rentalHistoryTable->setFocusPolicy(Qt::NoFocus);
    ui->rentalHistoryTable->verticalHeader()->setVisible(false);
    ui->rentalHistoryTable->setShowGrid(false);
    ui->rentalHistoryTable->setAlternatingRowColors(false);
    ui->rentalHistoryTable->horizontalHeader()->setStretchLastSection(true);
    ui->rentalHistoryTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    ui->rentalHistoryTable->verticalHeader()->setDefaultSectionSize(48);
}
void userDashboard::applyFilter(){
    ui->fleetTable->setRowCount(0);
    string ftype = ui->typeComboBox->currentText().toStdString();
    string fman = ui->manufacturerComboBox->currentText().toStdString();
    string fFuel = ui->fuelComboBox->currentText().toStdString();
    bool favail = ui->availabilityCheckBox->isChecked();
    int anyMatch = 0;
    for(int j = 0; j < i->getCarCount(); j++){
        Car *car = i->getCarAt(j);
        if(ftype != "All Types" && car->getType() != ftype)
            continue;
        if(fman != "All Manufacturers" && car->getCompany() != fman)
            continue;
        if(fFuel == "Electric" && car->getFuelType() != 1)
            continue;
        if(fFuel == "Petrol" && car->getFuelType() != 0)
            continue;
        if(favail && !car->isAvailable())
            continue;
        int row = ui->fleetTable->rowCount();
        ui->fleetTable->insertRow(row);
        string fuelString;
        if(car->getFuelType() == 1)
            fuelString = "Electric";
        else
            fuelString = "Petrol";
        QTableWidgetItem *name = new QTableWidgetItem(QString::fromStdString(car->getName()));
        QTableWidgetItem *type = new QTableWidgetItem(QString::fromStdString(car->getType()));
        QTableWidgetItem *manufacturer = new QTableWidgetItem(QString::fromStdString(car->getCompany()));
        QTableWidgetItem *fuel = new QTableWidgetItem(QString::fromStdString(fuelString));
        QTableWidgetItem *mileage = new QTableWidgetItem(QString::fromStdString(car->getSpec()));
        QTableWidgetItem *price = new QTableWidgetItem("Rs " + QString::fromStdString(Inventory::formatPrice(car->getPrice())));
        name->setData(Qt::UserRole, j);
        ui->fleetTable->setItem(row, 0, name);
        ui->fleetTable->setItem(row, 1, type);
        ui->fleetTable->setItem(row, 2, manufacturer);
        ui->fleetTable->setItem(row, 3, fuel);
        ui->fleetTable->setItem(row, 4, mileage);
        ui->fleetTable->setItem(row, 5, price);
        anyMatch++;
    }
        ui->resultLabel->setText(QString::number(anyMatch) + " vehicles found");
}
void userDashboard::loadFromHistory(){
    qDebug() << "loadFromHistory called";
    qDebug() << "Rental count:" << c.getRentalHistory().getNoOfRentals();
    qDebug() << "Table row count before clear:" << ui->rentalHistoryTable->rowCount();
    ui->rentalHistoryTable->setRowCount(0);
    qDebug() << "Table column count:" << ui->rentalHistoryTable->columnCount();
    for(int j = 0; j < c.getRentalHistory().getNoOfRentals(); j++){
        rental r = c.getRentalHistory().getRentalAt(j);
        int row = ui->rentalHistoryTable->rowCount();
        ui->rentalHistoryTable->insertRow(row);
        QTableWidgetItem *rentalID = new QTableWidgetItem(QString::fromStdString(r.getRentalID()));
        QTableWidgetItem *carName = new QTableWidgetItem(QString::fromStdString(r.getCardID()));
        QTableWidgetItem *userName = new QTableWidgetItem(QString::fromStdString(c.getFullName()));
        QTableWidgetItem *startDate = new QTableWidgetItem(QString::fromStdString(r.getStartDate()));
        QTableWidgetItem *endDate = new QTableWidgetItem(QString::fromStdString(r.getEndDate()));
        QTableWidgetItem *money = new QTableWidgetItem("Rs. " + QString::fromStdString(Inventory::formatPrice(r.getRentalPrice())));
        QTableWidgetItem *statusContent = new QTableWidgetItem(QString::fromStdString(r.getStatus()));
        string status = r.getStatus();
        if(status == "Active"){
            statusContent->setBackground(QBrush(QColor("#f0fdf4")));
            statusContent->setForeground(QBrush(QColor("#16a34a")));
        }
        if(status == "Upcoming"){
            statusContent->setBackground(QBrush(QColor("#eff6ff")));
            statusContent->setForeground(QBrush(QColor("#2563eb")));
        }
        if(status == "Completed"){
            statusContent->setBackground(QBrush(QColor("#f4f5f8")));
            statusContent->setForeground(QBrush(QColor("#6b7280")));
        }
        if(status == "Cancelled"){
            statusContent->setBackground(QBrush(QColor("#fef2f2")));
            statusContent->setForeground(QBrush(QColor("#dc2626")));
        }
        if(status == "Not Returned"){
            statusContent->setBackground(QBrush(QColor("#fff7ed")));
            statusContent->setForeground(QBrush(QColor("#ea580c")));
        }
        ui->rentalHistoryTable->setItem(row, 0, rentalID);
        ui->rentalHistoryTable->setItem(row, 1, userName);
        ui->rentalHistoryTable->setItem(row, 2, carName);
        ui->rentalHistoryTable->setItem(row, 3, startDate);
        ui->rentalHistoryTable->setItem(row, 4, endDate);
        ui->rentalHistoryTable->setItem(row, 5, money);
        ui->rentalHistoryTable->setItem(row, 6, statusContent);
    }
}
void userDashboard::onFleetRowClicked(int r, int c){
    QTableWidgetItem *startCell = ui->fleetTable->item(r, 0);
    if(!startCell)
        return;
    int inevtnoryIndex = startCell->data(Qt::UserRole).toInt();
    Car *car = i->getCarAt(inevtnoryIndex);
    goToBooking(car);
}
void userDashboard::goToBooking(Car* car){
    QString type = QString::fromStdString(car->getType());
    int typeIndex = ui->typeBooking->findText(type);
    if(typeIndex >= 0)
        ui->typeBooking->setCurrentIndex(typeIndex);
    QString name = QString::fromStdString(car->getName());
    int nameIndex = ui->carBooking->findText(name);
    if(nameIndex >= 0)
        ui->carBooking->setCurrentIndex(nameIndex);
    ui->stackedWidget->setCurrentWidget(ui->newBookingPage);
}
void userDashboard::initializeBookingPage(){
    userSelection = nullptr;
    discount = 0.0;
    ui->typeBooking->addItem("All Types");
    StringArray typeL = i->getTypes();
    for(int j = 0; j < typeL.size(); j++){
        ui->typeBooking->addItem(QString::fromStdString(typeL[j]));
    }
    ui->pickupDate->setDate(QDate::currentDate());
    ui->dropOffDate->setDate(QDate::currentDate().addDays(1));
    ui->pickupDate->setMinimumDate(QDate::currentDate());
    ui->dropOffDate->setMinimumDate(QDate::currentDate().addDays(1));
    connect(ui->typeBooking, SIGNAL(currentIndexChanged(int)), this, SLOT(whenCarTypeChanged(int)));
    connect(ui->carBooking, SIGNAL(currentIndexChanged(int)), this, SLOT(whenCarSelected(int)));
    connect(ui->pickupDate, &QDateEdit::dateChanged, this, [this](){
        QDate date = ui->pickupDate->date();
        ui->dropOffDate->setMinimumDate(date.addDays(1));
        if(ui->dropOffDate->date() <= date)
            ui->dropOffDate->setDate(date.addDays(1));
        updateBooking();
    });
    connect(ui->dropOffDate, &QDateEdit::dateChanged, this, [this](){
        updateBooking();
    });
    connect(ui->deliveryCheck, &QCheckBox::toggled, this, [this](){
        updateBooking();
    });
    connect(ui->insuranceCheck, &QCheckBox::toggled, this, [this](){
        updateBooking();
    });
    connect(ui->driverCheck, &QCheckBox::toggled, this, [this](){
        updateBooking();
    });
}
void userDashboard::whenCarTypeChanged(int index){
    ui->carBooking->clear();
    ui->carBooking->addItem("All Cars");
    userSelection = nullptr;
    string typeSelected = ui->typeBooking->currentText().toStdString();
    for(int j = 0; j < i->getCarCount(); j++){
        Car *newCar = i->getCarAt(j);
        if(typeSelected == "All Types"){
            ui->carBooking->addItem(QString::fromStdString(newCar->getName()), QVariant(j));
        }
        else{
            if(typeSelected == newCar->getType()){
                ui->carBooking->addItem(QString::fromStdString(newCar->getName()), QVariant(j));
            }
            else
                continue;
        }
    }
}
void userDashboard::whenCarSelected(int index){
    if(index <= 0){
        userSelection = nullptr;
        updateBooking();
        return;
    }
    userSelection = i->getCarAt(ui->carBooking->currentData().toInt());
    updateBooking();
}
void userDashboard::updateBooking(){
    if(userSelection == nullptr){
        ui->priceSummaryLabel->setText("N/A");
        ui->durationLabelSummary->setText("N/A");
        ui->durationLabel->setText("Duration: N/A");
        ui->addOnLabel->setText("N/A");
        ui->taxLabel->setText("N/A");
        ui->totalPriceLabel->setText("N/A");
    }
    else{
        int duration = ui->pickupDate->date().daysTo(ui->dropOffDate->date());
        double price = userSelection->getPrice() * duration;
        double addOnPrice = 0.0;
        if(ui->deliveryCheck->isChecked()){
            addOnPrice += 2000 * duration;
        }
        if(ui->insuranceCheck->isChecked()){
            addOnPrice += 2000 * duration;
        }
        if(ui->driverCheck->isChecked()){
            addOnPrice += 1000 * duration;
        }
        double tax = 0.05 * (price + addOnPrice);
        double total = (price + addOnPrice + tax) * (1.0 - discount);
        ui->priceSummaryLabel->setText("Rs " + QString::fromStdString(Inventory::formatPrice(price)));
        ui->durationLabelSummary->setText(QString::number(duration) + " Days");
        ui->durationLabel->setText("Duration: " + QString::number(duration) + " Days");
        ui->addOnLabel->setText("Rs " + QString::fromStdString(Inventory::formatPrice(addOnPrice)));
        ui->taxLabel->setText("Rs " + QString::fromStdString(Inventory::formatPrice(tax)));
        ui->totalPriceLabel->setText("Rs " + QString::fromStdString(Inventory::formatPrice(total)));

    }
}
void userDashboard::on_confirmBtn_clicked(){
    if(userSelection == nullptr){
        QMessageBox::warning(this, "Error", "Please select a car to book!");
        return;
    }
    int duration = ui->pickupDate->date().daysTo(ui->dropOffDate->date());
    double price = userSelection->getPrice() * duration;
    double addOnPrice = 0.0;
    if(ui->deliveryCheck->isChecked()){
        addOnPrice += 2000 * duration;
    }
    if(ui->insuranceCheck->isChecked()){
        addOnPrice += 2000 * duration;
    }
    if(ui->driverCheck->isChecked()){
        addOnPrice += 1000 * duration;
    }
    double tax = 0.05 * (price + addOnPrice);
    double total = (price + addOnPrice + tax) * (1.0 - discount);
    string startDate = ui->pickupDate->date().toString("dd/MM/yyyy").toStdString();
    string endDate = ui->dropOffDate->date().toString("dd/MM/yyyy").toStdString();
    string rentalID = "R";
    for(int j = 0; j < 4; j++){
        int num = rand() % 10;
        rentalID += to_string(num);
    }
    rental r(rentalID, c.getUserName(), startDate, endDate, userSelection->getCardId(), total, "Upcoming");
    c.getRentalHistory().appendNewRental(r);
    ui->typeBooking->setCurrentIndex(0);
    ui->carBooking->clear();
    ui->driverCheck->setChecked(false);
    ui->insuranceCheck->setChecked(false);
    ui->deliveryCheck->setChecked(false);
    ui->discountEdit->clear();
    discount = 0.0;
    userSelection = nullptr;
    updateBooking();
    ui->stackedWidget->setCurrentWidget(ui->invoicesPage);
}
void userDashboard::on_discountBtn_clicked(){
    string promoCode = ui->discountEdit->text().toStdString();
    if(promoCode == "SAVE10"){
        discount = 0.10;
        QMessageBox::information(this, "Congrats", "You just recieved a 10 percent discount");
    }
    else if(promoCode == "SAVE20"){
        discount = 0.20;
        QMessageBox::information(this, "Congrats", "You just recieved a 20 percent discount");
    }
    else{
        discount = 0.0;
        QMessageBox::warning(this, "Invalid", "Invalid Promo Code Entered");
    }
    updateBooking();
}
userDashboard::~userDashboard()
{
    delete ui;
}
