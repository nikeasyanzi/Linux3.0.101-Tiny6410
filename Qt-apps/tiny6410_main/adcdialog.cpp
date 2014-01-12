// #define DEBUG

#ifndef DEBUG
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/fs.h>
#include <errno.h>
#include <string.h>
#endif

#include <QMessageBox>
#include "adcdialog.h"
#include "ui_adcdialog.h"

adcDialog::adcDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::adcDialog)
{
    ui->setupUi(this);

    QObject::connect(ui->adcRefreshButton, SIGNAL(clicked()), this, SLOT(changeAdcValue()));
}

adcDialog::~adcDialog()
{
    delete ui;
}

void adcDialog::changeAdcValue(void)
{
#ifndef DEBUG
    char buffer[30];
    int length;
    int adcValue = -1;
    int adcFd;

    adcFd = ::open("/dev/tiny6410_adc", 0);
    if (adcFd < 0) {
        ::perror("open ADC device : \n");
        ::exit(1);
    }

    length = ::read(adcFd, buffer, sizeof buffer -1);
    if(length > 0)
    {
        buffer[length] = '\0';
        ::sscanf(buffer, "%d", &adcValue);
    }

    ui->adcValue->setText(QString::number(adcValue));

    ::close(adcFd);
#endif
}
