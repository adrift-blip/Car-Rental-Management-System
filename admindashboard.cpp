#include "admindashboard.h"
#include "ui_admindashboard.h"

AdminDashboard::AdminDashboard(Inventory* inv, admin &a, QWidget* parent)
    : QWidget(parent),
    ui(new Ui::AdminDashboard),
    inventory(inv), a(a)
{
    ui->setupUi(this);
    setupTables();
    updateOverviewStats();
    connectSignals();

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
}

AdminDashboard::~AdminDashboard()
{
    delete ui;
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
    connect(ui->overviewBtn,    &QPushButton::clicked, this, &AdminDashboard::onOverviewClicked);
    connect(ui->maintBtn,       &QPushButton::clicked, this, &AdminDashboard::onMaintClicked);
    connect(ui->availBtn,       &QPushButton::clicked, this, &AdminDashboard::onAvailClicked);
    connect(ui->allCarsBtn,     &QPushButton::clicked, this, &AdminDashboard::onAllCarsClicked);
    connect(ui->toggleMaintBtn, &QPushButton::clicked, this, &AdminDashboard::onToggleMaintenance);
    connect(ui->toggleAvailBtn, &QPushButton::clicked, this, &AdminDashboard::onToggleAvailability);
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
    int total       = inventory->getCarCount();
    int onMaint     = 0;
    int unavail     = 0;
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
    QPushButton* navBtns[4];
    navBtns[0] = ui->overviewBtn;
    navBtns[1] = ui->maintBtn;
    navBtns[2] = ui->availBtn;
    navBtns[3] = ui->allCarsBtn;

    for (int i = 0; i < 4; i++) {
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
    ui->stackedWidget->setCurrentIndex(3);
    setSidebarActive(ui->allCarsBtn);
    populateAllCarsTable();
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
