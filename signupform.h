#ifndef SIGNUPFORM_H
#define SIGNUPFORM_H

#include <QWidget>
#include "user.h"
namespace Ui {
class signUpForm;
}

class signUpForm : public QWidget
{
    Q_OBJECT

public:
    explicit signUpForm(QWidget *parent = nullptr);
    ~signUpForm();
private slots:
    void on_signUpButton_clicked();
    void on_tcsButton_clicked();
    void on_backButton_clicked();
private:
    Ui::signUpForm *ui;
    User currentUser;
};
#endif // SIGNUPFORM_H
