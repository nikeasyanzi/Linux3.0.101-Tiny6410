// #define DEBUG

#ifndef DEBUG
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#endif

#include <QMessageBox>
#include "leddialog.h"
#include "ui_leddialog.h"

ledDialog::ledDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ledDialog)
{
    ui->setupUi(this);

    /* connect ui buttons to slot */
    QObject::connect(ui->pushButtonLed0, SIGNAL(clicked()), this, SLOT(tiny6410LedSlot()));
    QObject::connect(ui->pushButtonLed1, SIGNAL(clicked()), this, SLOT(tiny6410LedSlot()));
    QObject::connect(ui->pushButtonLed2, SIGNAL(clicked()), this, SLOT(tiny6410LedSlot()));
    QObject::connect(ui->pushButtonLed3, SIGNAL(clicked()), this, SLOT(tiny6410LedSlot()));
    QObject::connect(ui->pushButtonRightWater, SIGNAL(clicked()), this, SLOT(tiny6410LedSlot()));
    QObject::connect(ui->pushButtonTurnOffAll, SIGNAL(clicked()), this, SLOT(tiny6410LedSlot()));
    QObject::connect(ui->pushButtonLeftWater, SIGNAL(clicked()), this, SLOT(tiny6410LedSlot()));
    QObject::connect(ui->pushButtonTurnOnAll, SIGNAL(clicked()), this, SLOT(tiny6410LedSlot()));
}

ledDialog::~ledDialog()
{
    delete ui;
}

int ledDialog::tiny6410LedSlot(){
#ifndef DEBUG
    QPushButton *sender= (QPushButton*)(this->sender());
    QMessageBox msgBox;
    int fd=0;

    fd = ::open("/dev/tiny6410_leds", 0);

    if(sender->objectName() == "pushButtonLed0")
    {
        ::ioctl(fd, 0, 0);
    }
    else if(sender->objectName() == "pushButtonLed1")
    {
        ::ioctl(fd, 0, 1);
    }
    else if(sender->objectName() == "pushButtonLed2")
    {
        ::ioctl(fd, 0, 3);
    }
    else if(sender->objectName() == "pushButtonLed3")
    {
        ::ioctl(fd, 0, 2);
    }
    else if(sender->objectName() == "pushButtonRightWater")
    {
        ::ioctl(fd, 5, 0);
        // msgBox.setText(tr("Not Support Now !"));
        // msgBox.exec();
    }
    else if(sender->objectName() == "pushButtonTurnOffAll")
    {
        ::ioctl(fd, 3, 0);
    }
    else if(sender->objectName() == "pushButtonLeftWater")
    {
        ::ioctl(fd, 4, 0);
        // msgBox.setText(tr("Not Support Now !"));
        // msgBox.exec();
    }
    else if(sender->objectName() == "pushButtonTurnOnAll")
    {
        ::ioctl(fd, 1, 0);
    }
    else
    {
        ::close(fd);
    }

    ::close(fd);
#endif

    return 0;
}
