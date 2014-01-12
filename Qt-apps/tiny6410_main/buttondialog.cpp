// #define DEBUG

#ifndef DEBUG
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>
#include <errno.h>
#endif

#include "buttondialog.h"
#include "ui_buttondialog.h"

buttonDialog::buttonDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::buttonDialog)
{
    ui->setupUi(this);
    QObject::connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(emitButtonsMsg()));
}

buttonDialog::~buttonDialog()
{
    delete ui;
}

void buttonDialog::buttonsChangeText(QString labelText[4])
{
    ui->label1->setText(labelText[0]);
    ui->label2->setText(labelText[1]);
    ui->label3->setText(labelText[2]);
    ui->label4->setText(labelText[3]);
}

void buttonDialog::emitButtonsMsg()
{
    emit buttonsMsgSlot();
}

int buttonDialog::buttonsMsgSlot()
{
#ifndef DEBUG
    QString labelText[4];
    int buttonsFd;
    char buttonsStatus[8] = {'0', '0', '0', '0', '0', '0', '0', '0'};

    buttonsFd = ::open("/dev/tiny6410_buttons", 0);
    if(buttonsFd < 0)
    {
        ::perror("open device buttons \n");
        ::exit(1);
    }

    if(read(buttonsFd, buttonsStatus, sizeof buttonsStatus) != sizeof buttonsStatus)
    {
        ::perror("read buttons: \n");
        ::exit(1);
    }

    for(unsigned char i=0; i<sizeof(buttonsStatus)/sizeof(buttonsStatus[0]); i++)
    {
        if('1' == buttonsStatus[i])
        {
            labelText[0] = "No.";
            labelText[1] = QString::number(i+1);
            labelText[2] = "Button's";
            labelText[3] = "Pressed";
            break;
        }
        else
        {
            labelText[0] = "Not Any";
            labelText[1] = "Button";
            labelText[2] = "Is";
            labelText[3] = "Pressed";
        }
    }
    buttonsChangeText(labelText);

    ::close(buttonsFd);
#endif

    return 0;
}
