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

QT_BEGIN_NAMESPACE
namespace Ui { class AdminDashboard; }
QT_END_NAMESPACE

class AdminDashboard : public QWidget {
    Q_OBJECT

public:
    AdminDashboard(Inventory* inv, admin &a, QWidget* parent = 0);
    ~AdminDashboard();

    void refreshAll();

private:
    Ui::AdminDashboard* ui;
    admin a;
    Inventory* inventory;
    void onOverviewClicked();
    void onMaintClicked();
    void onAvailClicked();
    void onAllCarsClicked();
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
};

#endif
