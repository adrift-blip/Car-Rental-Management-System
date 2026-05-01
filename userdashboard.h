#ifndef USERDASHBOARD_H
#define USERDASHBOARD_H

#include <QWidget>
#include "inventory.h"
#include "rental.h"
#include "customer.h"
#include "invoice.h"
#include "Payment.h"

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
    void on_overviewBtn_clicked();
    void on_invoiceBtn_clicked();
    void on_logoutBtn_clicked();
    void initializeFleet();
    void applyFilter();
    void onFleetRowClicked(int r, int c);
    void goToBooking(Car* car);
    void createHistoryTable();
    void loadFromHistory();
    void initializeBookingPage();
    void whenCarTypeChanged(int index);
    void whenCarSelected(int index);
    void updateBooking();
    void on_confirmBtn_clicked();
    void on_discountBtn_clicked();
private:
    Ui::userDashboard *ui;
    customer c;
    Inventory* i;
    Car* userSelection;
    double discount;
    void populateInvoicePage(const Invoice& inv, const Payment& pay);
};

#endif // USERDASHBOARD_H
