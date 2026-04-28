#include "signinform.h"
#include "userdashboard.h"
#include "signupform.h"
#include <iostream>
#include <string>
#include "customer.h"
#include "ui_signinform.h"
#include <fstream>
#include <sstream>
#include "user.h"
#include "admin.h"
#include "inventory.h"
#include "admindashboard.h"
#include<QMessageBox>
using namespace std;
signInForm::signInForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::signInForm)
{
    this->setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
}
void signInForm::on_login_clicked() {
    string username = ui->usernameEdit->text().toStdString();
    string password = ui->passwordEdit->text().toStdString();
    if(username.empty() || password.empty()){
        QMessageBox::warning(this, "Error", "Please fill in all fields");
    return;
    }
    else{
        if(ui->loginAsAdmin->isChecked()){
            currentUser.setUserName(username);
            currentUser.setUserPassword(password);
            currentUser.setPrivilegeLevel(1);
        }
        else if(ui->loginAsUser->isChecked()){
            currentUser.setUserName(username);
            currentUser.setUserPassword(password);
            currentUser.setPrivilegeLevel(0);
        }
        else{
            QMessageBox::warning(this, "Error", "Please fill in all fields");
            return;
        }
        if(currentUser.loginValid()){
            if(currentUser.getPrivilegeLevel() == 0){
                ifstream inFile("../../data/users.txt");
                if(!inFile){
                    cout << "File did not open";
                    exit;
                }
                string line, userPassword, userPrivelege, fullName, liscenseNum, loyaltyPoints, currentUserName;
                while (getline(inFile, line)) {
                    stringstream sample(line);
                    getline(sample, currentUserName, ',');
                    if (currentUserName == currentUser.getUserName()) {
                        getline(sample, userPassword, ',');
                        getline(sample, userPrivelege, ',');
                        getline(sample, fullName, ',');
                        getline(sample, liscenseNum, ',');
                        getline(sample, loyaltyPoints);
                    }
                }
                customer c(currentUser.getUserName(), currentUser.getUserPassword(), fullName, liscenseNum, stoi(loyaltyPoints));
                qDebug() << "Rental count after construction:" << c.getRentalHistory().getNoOfRentals();
                userDashboard *cd = new userDashboard(c);
                cd->show();
                this->hide();
            }
            if(currentUser.getPrivilegeLevel() == 1){
                ifstream inFile("../../data/users.txt");
                if(!inFile){
                    cout << "File did not open";
                    exit;
                }
                string line, userPassword, userPrivelege, fullName, empID, currentUserName;
                while (getline(inFile, line)) {
                    stringstream sample(line);
                    getline(sample, currentUserName, ',');
                    if (currentUserName == currentUser.getUserName()) {
                        getline(sample, userPassword, ',');
                        getline(sample, userPrivelege, ',');
                        getline(sample, fullName, ',');
                        getline(sample, empID);
                    }
                }
                admin a(currentUser.getUserName(), currentUser.getUserPassword(), 1, fullName, empID);
                Inventory *i = new Inventory();
                i->loadFromFile("../../data/inventory.txt");
                AdminDashboard *ad = new AdminDashboard(i, a);
                ad->show();
                this->hide();
            }
        }
        else{
            QMessageBox::warning(this, "Error!", "User not identified! Please sign up");
        }
    }
}
void signInForm:: on_signUp_clicked(){
    signUpForm *signUp = new signUpForm();
    signUp->show();
    this->hide();
}
signInForm::~signInForm()
{
    delete ui;
}
