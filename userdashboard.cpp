#include "userdashboard.h"
#include "ui_userdashboard.h"
#include <ctime>
#include <QTimer>
#include "inventory.h"
#include "signinform.h"
#include <string.h>
#include <QString>
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
    headers << "Name" << "Type" << "Brand" << "Fuel" << "Mileage" << "Price/km";
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
        ui->resultLabel->setText(QString::number(anyMatch) + " vehicles found");
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
    ui->stackedWidget->setCurrentWidget(ui->newBookingPage);
}
userDashboard::~userDashboard()
{
    delete ui;
}
