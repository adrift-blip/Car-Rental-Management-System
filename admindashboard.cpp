#include "admindashboard.h"
#include "ui_admindashboard.h"
#include <fstream>
#include <ctime>
AdminDashboard::AdminDashboard(Inventory* inv, admin &a, QWidget* previousWindow, QWidget* parent)
    : QWidget(parent),
    ui(new Ui::AdminDashboard), prev(previousWindow),
    inventory(inv), a(a), allRentals(nullptr)
{
    ui->setupUi(this);
    setupTables();
    updateOverviewStats();
    connectSignals();
    ui->mileageEdit->setValidator(new QDoubleValidator(0.0, 9999999.0, 2, this));
    ui->priceEdit->setValidator(new QIntValidator(0, 9999999, this));
    ui->stackedWidget->setCurrentIndex(0);
    setSidebarActive(ui->overviewBtn);
    string fullName = this->a.getFullName(); string initials;
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
    ui->adminAvatarLabel->setText(qInitials);
    ui->adminNameLabel->setText(QString::fromStdString(fullName));
}

AdminDashboard::~AdminDashboard()
{
    delete ui;
    delete allRentals;
}

void AdminDashboard::refreshAll()
{
    updateOverviewStats();
    populateMaintTable();
    populateAvailTable();
    populateAllCarsTable();
}

void AdminDashboard::connectSignals()
{
    connect(ui->overviewBtn, &QPushButton::clicked, this, &AdminDashboard::onOverviewClicked);
    connect(ui->maintBtn, &QPushButton::clicked, this, &AdminDashboard::onMaintClicked);
    connect(ui->availBtn, &QPushButton::clicked, this, &AdminDashboard::onAvailClicked);
    connect(ui->allCarsBtn, &QPushButton::clicked, this, &AdminDashboard::onAllCarsClicked);
    connect(ui->toggleMaintBtn, &QPushButton::clicked, this, &AdminDashboard::onToggleMaintenance);
    connect(ui->toggleAvailBtn, &QPushButton::clicked, this, &AdminDashboard::onToggleAvailability);
    connect(ui->addAdminBtn, &QPushButton::clicked, this, &AdminDashboard::onAddAdminClicked);
    connect(ui->confirmBtn, &QPushButton::clicked, this, &AdminDashboard::on_confirmBtn_Clicked);
    connect(ui->AddRemoveCar, &QPushButton::clicked, this, &AdminDashboard::onAddRemoveCarClicked);
    connect(ui->addCarButton, &QPushButton::clicked, this, &AdminDashboard::on_addCarButton_Clicked);
    connect(ui->removeCarButton, &QPushButton::clicked, this, &AdminDashboard::on_removeCarButton_Clicked);
    connect(ui->carIDComboBox, &QComboBox::currentIndexChanged, this, &AdminDashboard::onCarRemoveSelected);
    connect(ui->manageRentalStatusBtn, &QPushButton::clicked, this, &AdminDashboard::onManageRentalClicked);
    connect(ui->idComboBox, &QComboBox::currentIndexChanged, this, &AdminDashboard::onRentalSelected);
    connect(ui->statusComboBox, &QComboBox::currentIndexChanged, this, [this](int){
        int index = ui->idComboBox->currentIndex();
        if(index < 0)
            return;
        allRentals->setStatusAt(index, ui->statusComboBox->currentText().toStdString());
        ofstream outFile("../../data/rental.txt");
        if(!outFile)
            return;
        for(int i = 0; i < allRentals->getNoOfRentals(); i++){
            rental r = allRentals->getRentalAt(i);
            outFile << r.getRentalID() << "," << r.getNameOfBooker() << "," << r.getStartDate() << "," << r.getEndDate() << "," << r.getCardID() << "," << r.getRentalPrice() << "," << r.getStatus() << "," << r.getHasDriver() << "," << r.getHasInsurance() << "," << r.getHasDelivery() << "," << r.getDiscountRate() << endl;
        }
    });
}

void AdminDashboard::configureTable(QTableWidget* t)
{
    const char* headers[] = {
        "#", "Type", "Company", "Name", "Fuel",
        "Spec", "Colour", "Available", "Maintenance", "Price/km"
    };
    const int colCount = 10;

    t->setColumnCount(colCount);

    for (int i = 0; i < colCount; i++) {
        t->setHorizontalHeaderItem(i, new QTableWidgetItem(headers[i]));
    }

    t->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    t->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);

    t->setSelectionBehavior(QAbstractItemView::SelectRows);
    t->setSelectionMode(QAbstractItemView::SingleSelection);
    t->setEditTriggers(QAbstractItemView::NoEditTriggers);
    t->verticalHeader()->setVisible(false);
    t->setAlternatingRowColors(true);
    t->setShowGrid(false);

    t->setStyleSheet(
        "QTableWidget {"
        "  background-color: #ffffff;"
        "  alternate-background-color: #F9FAFB;"
        "  border: 1px solid #E5E7EB;"
        "  border-radius: 8px;"
        "  font-size: 12px;"
        "  color: #374151;"
        "}"
        "QTableWidget::item:selected {"
        "  background-color: #EFF6FF;"
        "  color: #1D4ED8;"
        "}"
        "QHeaderView::section {"
        "  background-color: #F3F4F6;"
        "  color: #6B7280;"
        "  font-size: 11px;"
        "  font-weight: bold;"
        "  border: none;"
        "  border-bottom: 1px solid #E5E7EB;"
        "  padding: 6px 8px;"
        "}"
        );
}

void AdminDashboard::setupTables()
{
    configureTable(ui->maintTable);
    configureTable(ui->availTable);
    configureTable(ui->allCarsTable);

    populateMaintTable();
    populateAvailTable();
    populateAllCarsTable();
}

void AdminDashboard::populateTable(QTableWidget* t)
{
    t->setRowCount(0);

    int total = inventory->getCarCount();

    for (int i = 0; i < total; i++) {
        Car* car = inventory->getCarAt(i);
        int row = t->rowCount();
        t->insertRow(row);

        QTableWidgetItem* numItem = new QTableWidgetItem(QString::number(i + 1));
        numItem->setTextAlignment(Qt::AlignCenter);
        t->setItem(row, 0, numItem);

        QTableWidgetItem* typeItem = new QTableWidgetItem(car->getType().c_str());
        typeItem->setTextAlignment(Qt::AlignCenter);
        t->setItem(row, 1, typeItem);

        QTableWidgetItem* compItem = new QTableWidgetItem(car->getCompany().c_str());
        compItem->setTextAlignment(Qt::AlignCenter);
        t->setItem(row, 2, compItem);

        QTableWidgetItem* nameItem = new QTableWidgetItem(car->getName().c_str());
        nameItem->setTextAlignment(Qt::AlignCenter);
        t->setItem(row, 3, nameItem);

        const char* fuelStr = (car->getFuelType() == 1) ? "EV" : "Petrol";
        QTableWidgetItem* fuelItem = new QTableWidgetItem(fuelStr);
        fuelItem->setTextAlignment(Qt::AlignCenter);
        t->setItem(row, 4, fuelItem);

        QTableWidgetItem* specItem = new QTableWidgetItem(car->getSpec().c_str());
        specItem->setTextAlignment(Qt::AlignCenter);
        t->setItem(row, 5, specItem);

        QTableWidgetItem* colItem = new QTableWidgetItem(car->getColour().c_str());
        colItem->setTextAlignment(Qt::AlignCenter);
        t->setItem(row, 6, colItem);

        // Available — green yes, red no
        QTableWidgetItem* availItem = new QTableWidgetItem(car->isAvailable() ? "Yes" : "No");
        availItem->setTextAlignment(Qt::AlignCenter);
        if (car->isAvailable())
            availItem->setForeground(QColor("#22C55E"));
        else
            availItem->setForeground(QColor("#EF4444"));
        t->setItem(row, 7, availItem);

        // Maintenance — red yes (needs it), green no (fine)
        QTableWidgetItem* maintItem = new QTableWidgetItem(car->needsMaintenance() ? "Yes" : "No");
        maintItem->setTextAlignment(Qt::AlignCenter);
        if (car->needsMaintenance())
            maintItem->setForeground(QColor("#EF4444"));
        else
            maintItem->setForeground(QColor("#22C55E"));
        t->setItem(row, 8, maintItem);

        std::string priceStr = Inventory::formatPrice(car->getPrice());
        QTableWidgetItem* priceItem = new QTableWidgetItem(priceStr.c_str());
        priceItem->setTextAlignment(Qt::AlignCenter);
        t->setItem(row, 9, priceItem);
    }
}

void AdminDashboard::populateMaintTable()  { populateTable(ui->maintTable);   }
void AdminDashboard::populateAvailTable()  { populateTable(ui->availTable);   }
void AdminDashboard::populateAllCarsTable(){ populateTable(ui->allCarsTable); }

void AdminDashboard::updateOverviewStats()
{
    int total = inventory->getCarCount();
    int onMaint = 0;
    int unavail = 0;
    int readyToRent = 0;

    for (int i = 0; i < total; i++) {
        Car* car = inventory->getCarAt(i);

        if (car->needsMaintenance())
            onMaint++;

        if (!car->isAvailable())
            unavail++;

        if (car->isAvailable() && !car->needsMaintenance())
            readyToRent++;
    }

    ui->totalCarsValue->setText(QString::number(total));
    ui->rentedValue->setText(QString::number(unavail));
    ui->maintValue->setText(QString::number(onMaint));
    ui->availableValue->setText(QString::number(readyToRent));
}

void AdminDashboard::setSidebarActive(QPushButton* active)
{
    QPushButton* navBtns[7];
    navBtns[0] = ui->overviewBtn;
    navBtns[1] = ui->maintBtn;
    navBtns[2] = ui->availBtn;
    navBtns[4] = ui->allCarsBtn;
    navBtns[3] = ui->addAdminBtn;
    navBtns[5] = ui->AddRemoveCar;
    navBtns[6] = ui->manageRentalStatusBtn;
    for (int i = 0; i < 6; i++) {
        navBtns[i]->setChecked(navBtns[i] == active);
    }
}

void AdminDashboard::onOverviewClicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    setSidebarActive(ui->overviewBtn);
    updateOverviewStats();
}

void AdminDashboard::onMaintClicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    setSidebarActive(ui->maintBtn);
    populateMaintTable();
}

void AdminDashboard::onAvailClicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    setSidebarActive(ui->availBtn);
    populateAvailTable();
}

void AdminDashboard::onAllCarsClicked()
{
    ui->stackedWidget->setCurrentIndex(4);
    setSidebarActive(ui->allCarsBtn);
    populateAllCarsTable();
}

void AdminDashboard::onAddAdminClicked(){
    ui->stackedWidget->setCurrentIndex(3);
    setSidebarActive(ui->addAdminBtn);
}

void AdminDashboard::onToggleMaintenance()
{
    int row = ui->maintTable->currentRow();

    if (row < 0) {
        QMessageBox::warning(this, "No Selection", "Please select a car from the list first.");
        return;
    }

    Car* car = inventory->getCarAt(row);
    bool newState = !car->needsMaintenance();
    car->setMaintenance(newState);
    inventory->saveToFile();

    std::string msg;
    if (newState)
        msg = "Maintenance flagged for: " + car->getName();
    else
        msg = "Maintenance cleared for: " + car->getName();
    QMessageBox msgBox;

    refreshAll();
}

void AdminDashboard::onToggleAvailability()
{
    int row = ui->availTable->currentRow();

    if (row < 0) {
        QMessageBox::warning(this, "No Selection", "Please select a car from the list first.");
        return;
    }

    Car* car = inventory->getCarAt(row);
    bool newState = !car->isAvailable();
    car->setAvailable(newState);
    inventory->saveToFile();

    std::string msg;
    if (newState)
        msg = car->getName() + " is now marked as Available.";
    else
        msg = car->getName() + " is now marked as Unavailable (rented).";

    QMessageBox::information(this, "Availability Updated", msg.c_str());
    refreshAll();
}

void AdminDashboard::on_confirmBtn_Clicked(){
    string username = ui->newAdminName->text().toStdString();
    string password = ui->newPassword->text().toStdString();
    string fullname = ui->newFullName->text().toStdString();
    string adminID = ui->newAdminID->text().toStdString();
    if(username.empty() || password.empty() || fullname.empty() || adminID.empty()){
        QMessageBox::warning(this, "Error", "Please fill in all fields");
        return;
    }
    else{
        admin newAdmin(username, password,1,fullname,adminID);
            if(!newAdmin.isStrongPassword(password)){
                QMessageBox::warning(this, "Error", "Password is not Strong.\n\nA strong password must have:\n"
                                                    "1. Atleast 1 uppercase.\n"
                                                    "2. Atleast 1 lowercase.\n"
                                                    "3. Atleast 1 digit.\n"
                                                    "4. Atleast 1 special character.\n"
                                                    "5. More than or equal to 8 chracters.");
                return;
            }
            if(newAdmin.signUp()){
                ofstream outFile("../../data/users.txt", ios::app);
                if (!outFile) {
                    throw invalid_argument("Unable to open file or maybe file does not exist");
                }
                outFile << username << "," << password << ",1," <<  fullname << "," << adminID << endl;
                QMessageBox::information(this,"Successful", "New Admin has been added sucessfully");
                ui->newAdminName->clear();
                ui->newPassword->clear();
                ui->newFullName->clear();
                ui->newAdminID->clear();
            }
            else
            {QMessageBox::warning(this, "Error", "This user already exists");
                return;}
    }
}

void AdminDashboard::onAddRemoveCarClicked(){
    ui->carIDComboBox->clear();
    for(int i = 0; i < inventory->getCarCount(); i++){
        Car *car = inventory->getCarAt(i);
        ui->carIDComboBox->addItem(QString::fromStdString(car->getCardId()), QString::fromStdString(car->getCardId()));
    }
    ui->stackedWidget->setCurrentIndex(5);
    setSidebarActive(ui->AddRemoveCar);
}

void AdminDashboard::on_addCarButton_Clicked(){
    int fuelType;
    string type = ui->typeComboBox->currentText().toStdString();
    string manufacturer = ui->manufacturerEdit->text().toStdString();
    string name = ui->carNameEdit->text().toStdString();
    string fuel = ui->fuelComboBox->currentText().toStdString();
    string color = ui->colorEdit->text().toStdString();
    if(manufacturer.empty() || name.empty() || color.empty() ||ui->mileageEdit->text().isEmpty() || ui->priceEdit->text().isEmpty()){
            QMessageBox::warning(this, "Error", "Please fill in all fields");
            return;
    }
    string mileage = ui->mileageEdit->text().toStdString();
    int price = ui->priceEdit->text().toInt();
    if(fuel == "Petrol"){
        fuelType = 0;
        mileage += "L";
    }
    else{
        fuelType = 1;
        mileage += "KWH";
    }
    string CarID = "C";
    for(int i = 0; i < 4; i++){
        int num = rand() % 10;
        CarID += to_string(num);
    }
    inventory->addCar(new Car(CarID, type, manufacturer, name, fuelType, mileage, color, "yes", "yes", price));
    inventory->saveToFile("../../data/inventory.txt");
    QMessageBox::information(this, "Success", "Car Added Successfully");
    ui->manufacturerEdit->clear();
    ui->carNameEdit->clear();
    ui->colorEdit->clear();
    ui->mileageEdit->clear();
    ui->priceEdit->clear();
    refreshAll();
}

void AdminDashboard::onCarRemoveSelected(int index){
    string fuel;
    if(index < 0){
        return;
    }
    else{
        Car *car = inventory->getCarAt(index);
        if(car == nullptr){
            return;
        }
        ui->manLabel->setText(QString::fromStdString(car->getCompany()));
        ui->mileLabel->setText(QString::fromStdString(car->getSpec()));
        ui->namLabel->setText(QString::fromStdString(car->getName()));
        ui->priceLabele->setText(QString::number(car->getPrice()));
        ui->colLabel->setText(QString::fromStdString(car->getColour()));
        if(car->getFuelType() == 1){
            fuel = "EV";
        }
        else
            fuel = "Petrol";
        ui->fueLabel->setText(QString::fromStdString(fuel));
    }
}

void AdminDashboard::on_removeCarButton_Clicked(){
    string carID = ui->carIDComboBox->currentData().toString().toStdString();
    if(carID.empty()){
        QMessageBox::warning(this, "Error", "Please Select a Car To Remove");
        return;
    }
    Car *car = inventory->getCarFromID(carID);
    if(car == nullptr){
        QMessageBox::warning(this, "Error", "No car found with the ID");
        return;
    }
    for(int i = 0; i < inventory->getCarCount(); i++){
        if(inventory->getCarAt(i) == car){
            inventory->removeCar(i);
            inventory->saveToFile();
            break;
        }
    }
    QMessageBox::information(this, "Success", "Car was removed succesfully");
    ui->carIDComboBox->clear();
    for(int i = 0; i < inventory->getCarCount(); i++){
        Car *car = inventory->getCarAt(i);
        ui->carIDComboBox->addItem(QString::fromStdString(car->getCardId()), QString::fromStdString(car->getCardId()));
    }
    refreshAll();
}

void AdminDashboard::onManageRentalClicked(){
    if(allRentals != nullptr){
        delete allRentals;
        allRentals = nullptr;
    }
    allRentals = new rentalHistory("../../data/rental.txt");
    ui->idComboBox->clear();
    for(int i = 0; i < allRentals->getNoOfRentals(); i++){
        rental temp = allRentals->getRentalAt(i);
        ui->idComboBox->addItem(QString::fromStdString(temp.getRentalID()), QString::fromStdString(temp.getRentalID()));
    }
    ui->stackedWidget->setCurrentIndex(6);
    setSidebarActive(ui->manageRentalStatusBtn);
}

void AdminDashboard::onRentalSelected(int index){
    if(index < 0)
        return;
    rental r = allRentals->getRentalAt(index);
    ui->statusComboBox->blockSignals(true);
    ui->statusComboBox->setCurrentText(QString::fromStdString(r.getStatus()));
    ui->statusComboBox->blockSignals(false);
}

void AdminDashboard::on_logoutBtn_clicked(){
    prev->show();
    this->close();
}
