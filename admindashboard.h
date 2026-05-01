#ifndef ADMINDASHBOARD_H
#define ADMINDASHBOARD_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QStackedWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QMessageBox>
#include <QColor>
#include "admin.h"
#include "Inventory.h"
#include "rentalhistory.h"

QT_BEGIN_NAMESPACE
namespace Ui { class AdminDashboard; }
QT_END_NAMESPACE

class AdminDashboard : public QWidget {
    Q_OBJECT

public:
    AdminDashboard(Inventory* inv, admin &a, QWidget* previousWindow, QWidget* parent = 0);
    ~AdminDashboard();

    void refreshAll();
private slots:
    void on_logoutBtn_clicked();
    void onManageRentalClicked();
    void onRentalSelected(int index);
    void onCarRemoveSelected(int index);
    void onAddRemoveCarClicked();
    void on_addCarButton_Clicked();
    void on_removeCarButton_Clicked();
    void onOverviewClicked();
    void onMaintClicked();
    void onAvailClicked();
    void onAllCarsClicked();
    void onAddAdminClicked();
    void onToggleMaintenance();
    void onToggleAvailability();
    void connectSignals();
    void configureTable(QTableWidget* t);
    void setupTables();
    void populateTable(QTableWidget* t);
    void populateMaintTable();
    void populateAvailTable();
    void populateAllCarsTable();
    void updateOverviewStats();
    void setSidebarActive(QPushButton* active);
    void on_confirmBtn_Clicked();
private:
    Ui::AdminDashboard* ui;
    QWidget* prev;
    admin a;
    Inventory* inventory;
    rentalHistory* allRentals;
};

#endif
