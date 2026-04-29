#include <QApplication>
#include "Inventory.h"
#include "admindashboard.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // Load inventory from file
    Inventory inventory;
    inventory.loadFromFile("inventory.txt");

    // Create and show the admin dashboard
    AdminDashboard dashboard(&inventory);
    dashboard.setWindowTitle("RentEase — Admin Dashboard");
    dashboard.resize(880, 700);
    dashboard.show();

    return app.exec();
}