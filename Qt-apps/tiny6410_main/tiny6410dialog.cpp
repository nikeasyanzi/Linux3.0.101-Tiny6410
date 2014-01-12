#include "tiny6410dialog.h"
#include "ui_tiny6410dialog.h"

tiny6410Dialog::tiny6410Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::tiny6410Dialog)
{
    tiny6410LedDialog = new ledDialog;
    tiny6410ButtonDialog = new buttonDialog;
    tiny6410PwmDialog = new pwmDialog;
    tiny6410AdcDialog = new adcDialog;

    ui->setupUi(this);

    /* without border */
    this->setWindowFlags(Qt::Dialog|Qt::FramelessWindowHint);

    QObject::connect(ui->ledButton, SIGNAL(clicked()), tiny6410LedDialog, SLOT(show()));
    QObject::connect(ui->keyButton, SIGNAL(clicked()), tiny6410ButtonDialog, SLOT(show()));
    QObject::connect(ui->pwmButton, SIGNAL(clicked()), tiny6410PwmDialog, SLOT(tiny6410PwmOpenSlot()));
    QObject::connect(ui->adcButton, SIGNAL(clicked()), tiny6410AdcDialog, SLOT(show()));
}

tiny6410Dialog::~tiny6410Dialog()
{
    delete tiny6410LedDialog;
    delete tiny6410ButtonDialog;
    delete tiny6410PwmDialog;
    delete tiny6410AdcDialog;
    delete ui;
}
