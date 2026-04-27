#ifndef USERDASHBOARD_H
#define USERDASHBOARD_H

#include <QWidget>
#include "inventory.h"
#include "customer.h"

namespace Ui {
class userDashboard;
}

class userDashboard : public QWidget
{
    Q_OBJECT

public:
    explicit userDashboard(customer c, QWidget *parent = nullptr);
    ~userDashboard();
private slots:
    void on_logoutBtn_clicked();
    void initializeFleet();
    void applyFilter();
    void onFleetRowClicked(int r, int c);
    void goToBooking(Car* car);
private:
    Ui::userDashboard *ui;
    customer c;
    Inventory* i;
};

#endif // USERDASHBOARD_H
