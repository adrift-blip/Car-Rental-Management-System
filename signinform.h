#ifndef SIGNINFORM_H
#define SIGNINFORM_H

#include <QWidget>
#include "user.h"
namespace Ui {
class signInForm;
}

class signInForm : public QWidget
{
    Q_OBJECT

public:
    explicit signInForm(QWidget *parent = nullptr);
    ~signInForm();
private slots:
    void on_login_clicked();
    void on_signUp_clicked();
private:
    Ui::signInForm *ui;
    User currentUser;
};

#endif // SIGNINFORM_H
