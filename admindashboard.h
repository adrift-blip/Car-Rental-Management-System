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
#include "Inventory.h"

QT_BEGIN_NAMESPACE
namespace Ui { class AdminDashboard; }
QT_END_NAMESPACE

class AdminDashboard : public QWidget {
    Q_OBJECT

public:
    AdminDashboard(Inventory* inv, QWidget* parent = 0);
    ~AdminDashboard();

    void refreshAll();

private:
    Ui::AdminDashboard* ui;
    Inventory* inventory;

    // navigation handlers
    void onOverviewClicked();
    void onMaintClicked();
    void onAvailClicked();
    void onAllCarsClicked();

    // action handlers
    void onToggleMaintenance();
    void onToggleAvailability();

    // setup helpers
    void connectSignals();
    void configureTable(QTableWidget* t);
    void setupTables();

    // population helpers
    void populateTable(QTableWidget* t);
    void populateMaintTable();
    void populateAvailTable();
    void populateAllCarsTable();

    void updateOverviewStats();
    void setSidebarActive(QPushButton* active);
};

#endif