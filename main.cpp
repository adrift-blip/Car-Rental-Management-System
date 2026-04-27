// This application uses Qt, licensed under LGPL v3
// Qt source code available at: https://www.qt.io/download-open-source
#include "signinform.h"
#include "userdashboard.h"
#include "signupform.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    signInForm w;
    w.show();
    return a.exec();
}
