// #define DEBUG

#ifndef DEBUG
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#endif

#include <QMessageBox>
#include "pwmdialog.h"
#include "ui_pwmdialog.h"

#define PWM_IOCTL_SET_FREQ		1
#define PWM_IOCTL_STOP			0

pwmDialog::pwmDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::pwmDialog)
{
    ui->setupUi(this);

    /* connect ui buttons to slot */
    QObject::connect(ui->pwmDownButton, SIGNAL(clicked()), this, SLOT(tiny6410PwmSlot()));
    QObject::connect(ui->pwmUpButton, SIGNAL(clicked()), this, SLOT(tiny6410PwmSlot()));
}

pwmDialog::~pwmDialog()
{  
    delete ui;
}

void pwmDialog::tiny6410PwmOpenSlot(void)
{
    show();
#ifndef DEBUG
    /* set pwm frequence */
    pwmFreq = 1000;

    /* open tiny6410 pwm device */
    pwmFd = ::open("/dev/tiny6410_pwm", 0);
    if(pwmFd < 0)
    {
        ::perror("open pwm_buzzer device");
        ::exit(1);
    }
    else
    {
        setBuzzerFreq(pwmFreq);
    }
#endif
}

void pwmDialog::closeEvent(QCloseEvent *event)
{
#ifndef DEBUG
    ::ioctl(pwmFd, PWM_IOCTL_STOP);
    ::close(pwmFd);
#endif
}


void pwmDialog::setBuzzerFreq(int freq)
{
#ifndef DEBUG
    /* set buzzer frequence */
    ioctrlRet = ioctl(pwmFd, PWM_IOCTL_SET_FREQ, freq);
    if(ioctrlRet < 0)
    {
        ::perror("set the frequency of the buzzer");
        ::exit(1);
    }
#endif
}


int pwmDialog::tiny6410PwmSlot()
{
    QPushButton *sender= (QPushButton*)(this->sender());
    QMessageBox msgBox;

#ifndef DEBUG
    if(sender->objectName() == "pwmDownButton")
    {
        if(pwmFreq > 11)
        {
            pwmFreq -= 10;
        }
        setBuzzerFreq(pwmFreq);
    }
    else if(sender->objectName() == "pwmUpButton")
    {
        if(pwmFreq < 20000)
        {
            pwmFreq += 10;
        }
        setBuzzerFreq(pwmFreq);
    }
#endif
    return 0;
}
