#include "signupform.h"
#include "personaldetailsform.h"
#include <QMessageBox>
#include "ui_signupform.h"
#include "user.h"
#include<string>
signUpForm::signUpForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::signUpForm)
{
    this->setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
}

void signUpForm:: on_signUpButton_clicked(){
    string username = ui->usernameEdit->text().toStdString();
    string password = ui->passwordEdit->text().toStdString();
    if(username.empty() || password.empty()){
        QMessageBox::warning(this, "Error", "Please fill in all fields");
        return;
    }
    else{
        if(ui->tcs->isChecked()){
            currentUser.setUserName(username);
            currentUser.setUserPassword(password);
            currentUser.setPrivilegeLevel(0);
        }
        else{
            QMessageBox::warning(this, "Error", "Please agree to the terms and conditions");
            return;
        }
        try {
            currentUser.signUp();
        } catch (const invalid_argument &e) {
            QMessageBox::warning(this, "Error", "User already exists!");
            return;
        }
        User *u = &currentUser;
        personalDetailsForm *pdf = new personalDetailsForm(u);
        pdf->show();
        this->hide();
    }
}
signUpForm::~signUpForm()
{
    delete ui;
}
