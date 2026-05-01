#ifndef PERSONALDETAILSFORM_H
#define PERSONALDETAILSFORM_H

#include <QWidget>
#include "user.h"
namespace Ui {
class personalDetailsForm;
}

class personalDetailsForm : public QWidget
{
    Q_OBJECT

public:
    explicit personalDetailsForm(User *u = nullptr, QWidget *parent = nullptr);
    ~personalDetailsForm();
private slots:
    void on_registerButton_clicked();
private:
    Ui::personalDetailsForm *ui;
    User currentUser;
};

#endif // PERSONALDETAILSFORM_H