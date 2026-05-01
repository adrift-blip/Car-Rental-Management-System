#include "personaldetailsform.h"
#include <QMessageBox>
#include <string>
#include "user.h"
#include "customer.h"
#include "userdashboard.h"
#include "ui_personaldetailsform.h"

personalDetailsForm::personalDetailsForm(User *u, QWidget* previousWindow, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::personalDetailsForm), currentUser(*u), prev(previousWindow)
{
    this->setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
}
void personalDetailsForm::on_registerButton_clicked(){
    string fullName = ui->fullNameEdit->text().toStdString();
    string liscenseNo = ui->liscenseEdit->text().toStdString();
    if(fullName.empty() || liscenseNo.empty()){
        QMessageBox::warning(this, "Error", "Please fill in the personal details");
        return;
    }
    customer c(currentUser.getUserName(), currentUser.getUserPassword(), fullName, liscenseNo, 0);
    c.addPersonalDetails();
    userDashboard *u = new userDashboard(c, prev);
    u->show();
    this->hide();
}
personalDetailsForm::~personalDetailsForm()
{
    delete ui;
}
