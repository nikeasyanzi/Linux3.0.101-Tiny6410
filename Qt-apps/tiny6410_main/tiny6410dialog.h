#ifndef TINY6410DIALOG_H
#define TINY6410DIALOG_H

#include <QDialog>
#include <QCloseEvent>
#include "leddialog.h"
#include "buttondialog.h"
#include "pwmdialog.h"
#include "adcdialog.h"

namespace Ui {
class tiny6410Dialog;
}

class tiny6410Dialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit tiny6410Dialog(QWidget *parent = 0);
    ~tiny6410Dialog();

private:
    Ui::tiny6410Dialog *ui;
    ledDialog *tiny6410LedDialog;
    buttonDialog *tiny6410ButtonDialog;
    pwmDialog *tiny6410PwmDialog;
    adcDialog *tiny6410AdcDialog;
};

#endif // TINY6410DIALOG_H
