#include "signupform.h"
#include "personaldetailsform.h"
#include <QMessageBox>
#include "ui_signupform.h"
#include "user.h"
#include<string>
#include "signinform.h"
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
            if(!currentUser.isStrongPassword(password)){
                QMessageBox::warning(this, "Error", "Password is not Strong.\n\nA strong password must have:\n"
                                     "1. Atleast 1 uppercase.\n"
                                     "2. Atleast 1 lowercase.\n"
                                     "3. Atleast 1 digit.\n"
                                     "4. Atleast 1 special character.\n"
                                     "5. More than or equal to 8 chracters.");
                return;
            }
        }
        else{
            QMessageBox::warning(this, "Error", "Please agree to the terms and conditions");
            return;
        }
        if(currentUser.signUp()){
        currentUser.setUserName(username);
        currentUser.setUserPassword(password);
        currentUser.setPrivilegeLevel(0);
        User *u = &currentUser;
        personalDetailsForm *pdf = new personalDetailsForm(u);
        pdf->show();
        this->hide();
       }
       else
       {QMessageBox::warning(this, "Error", "This user already exists");
       return;}
    }
}
signUpForm::~signUpForm()
{
    delete ui;
}
void signUpForm::on_tcsButton_clicked() {
    QMessageBox::information(this, "Terms and Conditions",
                             "1. Drivability\nYou must be human.\n\n"
                             "2. Eligibility\nYou must be at least 18 and hold a valid license.\n\n"
                             "3. Reservations\nAll bookings are subject to availability.\n\n"
                             "4. Payment\nFull payment required at time of booking.\n\n"
                             "5. Liability\nRenter is responsible for any damage during rental.\n\n"
                             "6. Returns\nVehicles must be returned on time and in same condition.\n\n"
                             "7. Privacy\nYour data is stored securely and not shared.\n\n"
                             "8. Loyalty Points\nPoints earned per rental, redeemable for discounts.");
}
void signUpForm::on_backButton_clicked() {
    signInForm *signIn = new signInForm();
    signIn->show();
    this->hide();
}