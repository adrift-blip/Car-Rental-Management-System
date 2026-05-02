// This application uses Qt, licensed under LGPL v3
// Qt source code available at: https://www.qt.io/download-open-source
#include "signinform.h"
#include "userdashboard.h"
#include "signupform.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("Car-Rental-Management-System");
    signInForm w;
    a.setStyleSheet("QMessageBox { background-color: #ffffff; color: #111111; } QMessageBox QLabel { color: #111111; } QMessageBox QPushButton { background-color: #F3F4F6; color: #111111; border: 1px solid #E5E7EB; border-radius: 6px; padding: 6px 16px; } QMessageBox QPushButton:hover { background-color: #E5E7EB; }");
    w.show();
    return a.exec();
}
